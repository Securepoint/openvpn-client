#ifndef USERINFO_H
#define USERINFO_H

#include <QtCore>

class UserInfo
{
public:
    static UserInfo* instance();

    QString currentThreadUser () {
        return this->threadUsername;
    }

    QString currentThreadDomain () {
        return this->threadDomain;
    }

    QString currentThreadSID () {
        return this->threadSID;
    }

    QString currentLogonUser () {
        return this->logonUsername;
    }

    QString currentLogonDomain () {
        return this->logonDomain;
    }

    QString currentLogonSID () {
        return this->logonSID;
    }

private:
    UserInfo();
    static UserInfo* thisInstance;

    QString threadUsername;
    QString logonUsername;

    QString threadSID;
    QString logonSID;

    QString threadDomain;
    QString logonDomain;
};

#endif // USERINFO_H
