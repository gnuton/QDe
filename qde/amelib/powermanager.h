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

#ifndef POWERMANAGER_H
#define POWERMANAGER_H

#include <QObject>
struct Property {
    QString name;
    bool added;
    bool removed;
};

class QDBusInterface;

class PowerManager : public QObject
{
    Q_OBJECT

public:
    explicit PowerManager(QObject *parent = 0);
    void update();

signals:
    void batteryDischarging(bool discharging);
    void batteryLevel(int percentage);

private slots:
    void listen(int,QList<Property>);

private:
    QDBusInterface *mDevice;
};

#endif // POWERMANAGER_H
