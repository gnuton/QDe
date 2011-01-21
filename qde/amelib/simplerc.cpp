#include <SimpleRC>
#include <QStringList>
#include <QFile>
#include <QIODevice>
#include <QTextStream>

AmeSimpleRC::AmeSimpleRC(const QString &fileName, char sep, QObject *parent)
	: QObject(parent),
	  m_fileName(fileName)
{
	//m_separator = sep;
	m_status = process(sep);
}

AmeSimpleRC::~AmeSimpleRC()
{
}

QString AmeSimpleRC::value(const QString &key, const QString &defValue)
{
	return map.value(key, defValue);
}

bool AmeSimpleRC::process(char sep)
{
	QFile file(m_fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;

	bool f = false;
	QTextStream in(&file);
	while (!in.atEnd()) {
		QStringList data = (in.readLine()).split(sep);
		if (data.size() == 2) {
			map.insert(data[0].trimmed(), data[1].trimmed());
			f = true;
		}
	}
	file.close();
	return f;
}


