#include <QtCore>
#include <QString>
#include <QProcess>
#include <QObject>
#include <User>

class AProcess : public QObject
{
	Q_OBJECT

public:
	enum UserProcessStatus {Running, WrongPassword, WrongCommand, NotRunning, Crashed};

	AProcess(const QString &name, QObject *parent = 0);
	virtual ~AProcess();

	bool sudoExec(const QString &pwd, const QString &cmd);
	int status();

	QString m_Output;

public slots:
	virtual void parseOutput();
	void onExit();
	void onError(QProcess::ProcessError);

private:
	void init();

	QProcess *p;
	AmeUser *user;
	int m_Status;

};

