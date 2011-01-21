#ifndef __DBUSADAPTOR_H
#define __DBUSADAPTOR_H

#include <QtCore/QObject>
#include <QtDBus>

class SysPref;

class DBusAdaptor : public QDBusAbstractAdaptor
{
	Q_OBJECT
	Q_CLASSINFO("D-Bus Interface", "org.freedesktop.AnticoPref.WMCtrl")

public:
	DBusAdaptor(SysPref *a, QObject *obj);
	~DBusAdaptor();

public slots:
	Q_NOREPLY void callFunction(int fEnumerator, int param);

protected:
	SysPref *app;	
};

#endif

