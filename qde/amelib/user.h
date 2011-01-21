//////////////////////////////////////////
//  File      : user.h			//
//  Written by: ludmiloff@gmail.com	//
//  Copyright : GPL2			//
//////////////////////////////////////////

#ifndef __AMEUSER_H
#define __AMEUSER_H

#include <Global>
#include <QObject>
#include <QString>
#include <QIcon>
#include <QProcess>

class AME_EXPORT AmeUser : public QObject
{
public:
        AmeUser(QObject *parent = 0);
        explicit AmeUser(const QString &name, QObject *parent = 0);

        AmeUser(const AmeUser &user);
        AmeUser& operator =(const AmeUser &user);

        ~AmeUser();

        bool isSuperUser();
        bool isSudoer();

        QString shell() const;
        QString loginName() const;
        QIcon loginPhoto() const;
        QString homeDir() const;
        QString password() const;

        void setLoginName(const QString &name);
        void setPassword(const QString &pass);

        static bool isValid(const QString &name);

private:
        class AmeUserPrivate;
        AmeUserPrivate * d;
};

class AME_EXPORT AmeUserProcess : public QObject
{
        Q_OBJECT

public:
        enum UserProcessStatus {Running, WrongPassword, WrongCommand,
                        NotRunning, Crashed, Finished, WaitForExit, NoWait};

        AmeUserProcess(QObject *parent = 0);
        AmeUserProcess(const QString &name, QObject *parent = 0);
        AmeUserProcess(AmeUser *u, QObject *parent = 0);
        ~AmeUserProcess();

        void setUser(const QString &name, const QString &pass);

        bool exec(const QString &cmd, int mode = NoWait);
        int status();

public slots:
        virtual void parseOutput();
        void onExit();

signals:
        void processFinished(int);

private:
        void init();

        QProcess *p, *p1;
        AmeUser *user;
        int m_Status;

};

#endif
