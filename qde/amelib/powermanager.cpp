/*
 *  Copyright 2011  Antonio Aloisio <gnuton@gnuton.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "powermanager.h"
#include <QtDBus>

Q_DECLARE_METATYPE(Property)
Q_DECLARE_METATYPE(QList<Property>)

/* DBus arguments need to handle the << and >> operators */
const QDBusArgument & operator<< (QDBusArgument &arg, const Property &change){
    arg.beginStructure();
    arg << change.name << change.added << change.removed;
    arg.endStructure();
    return arg;
}

const QDBusArgument & operator>> (const QDBusArgument &arg, Property &change)
{
    arg.beginStructure();
    arg >> change.name >> change.added >> change.removed;
    arg.endStructure();
    return arg;
}


PowerManager::PowerManager(QObject *parent) :
    QObject(parent)
{
    qDBusRegisterMetaType< Property >();
    qDBusRegisterMetaType< QList<Property> >();

    QDBusConnection conn = QDBusConnection::systemBus();

    // Connect to the HAL Manager device to find all devices
    QDBusInterface hal("org.freedesktop.Hal",
		       "/org/freedesktop/Hal/Manager",
		       "org.freedesktop.Hal.Manager",
		       conn);

    QDBusMessage msg = hal.call("FindDeviceByCapability", "battery");
    QList<QVariant> devices = msg.arguments();
    qDebug() << "DEVICES" << devices;
    if(devices.isEmpty() || devices.first().toStringList().isEmpty()){
	qWarning() << "No battery found";
	return;
    }

    QString bat = devices.first().toStringList()[0];
    qDebug() << "Found device:" << bat;
    mDevice = new QDBusInterface("org.freedesktop.Hal", bat,
				 "org.freedesktop.Hal.Device",
				 conn);
    bool ok = conn.connect("org.freedesktop.Hal",
					      bat,
					      "org.freedesktop.Hal.Device",
					      "PropertyModified",
					      this,
					      SLOT(listen(int,QList<Property>)));
    if (!ok)
	qWarning() << "Unable to listen for battery's signals";

}

void PowerManager::listen(int i ,QList<Property> list)
{
    foreach(Property p, list){
	//qDebug() << i << p.name << p.added << p.removed;
	if (p.removed) continue;
	if (p.name == "battery.rechargeable.is_discharging" ||
	    p.name == "battery.charge_level.percentage"){
	    QDBusMessage msg = mDevice->call("GetProperty", p.name);
	    QVariant value = msg.arguments()[0];
	    switch(value.type()){
		case QVariant::Bool: emit batteryDischarging(value.toBool()); break;
		case QVariant::Int: emit batteryLevel(value.toInt()); break;
		default: qWarning() << "PowerManager: value type not handled";
	    }
	}
    }
}

void PowerManager::update(){
    QStringList interfaces;
    interfaces << "battery.rechargeable.is_discharging"
	       << "battery.charge_level.percentage";

    foreach(QString interface, interfaces){
	QDBusMessage msg = mDevice->call("GetProperty", interface);
	QVariant value = msg.arguments()[0];
	switch(value.type()){
	  case QVariant::Bool: emit batteryDischarging(value.toBool()); break;
	  case QVariant::Int: emit batteryLevel(value.toInt()); break;
	  default: qWarning() << "PowerManager: value type not handled";
	}
    }
}
