#ifndef __AMESIMPLERC_H
#define __AMESIMPLERC_H

#include <Global>
#include <QObject>
#include <QString>
#include <QMap>

class AME_EXPORT AmeSimpleRC : public QObject
{
public:
	AmeSimpleRC(const QString &fileName, char sep, QObject *parent = 0);
	~AmeSimpleRC();

	QString value(const QString &key, const QString &defValue = "");

	inline bool ready() { return m_status; };

protected:
	QString m_fileName;
	bool m_status;
	QMap <QString, QString> map;

	bool process(char sep);
};

#endif

