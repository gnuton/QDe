#include "dbusadapt.h"
#include "sysprefwindow.h"

DBusAdaptor::DBusAdaptor(SysPref *a, QObject *obj)
	: QDBusAbstractAdaptor(obj)
{
	app = a;
}

DBusAdaptor::~DBusAdaptor()
{
}

void DBusAdaptor::callFunction(int fEnumerator, int param)
{
	qDebug() << "RECEIVED DBUS MESSAGE : callFunction (" << fEnumerator << ")";
	app->remoteFunction(fEnumerator, param);
}
