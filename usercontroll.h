#ifndef USERCONTROLL_H
#define USERCONTROLL_H

#include <QtCore>

// Crypto-Klasse
#include "crypt.h"


class UserControll
{
public:
    // Für das Singleton
    static UserControll *getInstance ();

    // Normale Methoden
    void setConfigDirectory (QString dir);
    bool isConfigUserDataAvailable ();
    void setPrivateKey (QString key);
    QString getPrivateKeyPub ();

    // Username und Kennwort bekommen
    QString getUserNamePlain ();
    QString getUserPasswordPlain ();

    // Username und Kennwort setzen
    void setUserNameCrypt (QString plainName);
    void setUserPasswordCrypt (QString plainPwd);

    // Sind Configs mit Crypted Data vorhanden
    void setconfigWithCryptIsAvailable (bool flag);
    bool isConfigWithCryptAvailable ();
    bool isUserNameConfigWithCryptAvailable ();
    bool isUserPasswordConfigWithCryptAvailable ();

private:
    // Für das Singleton
    UserControll();
    static UserControll *mInst;
    bool configWithCryptIsAvailable;

    // Member
    QString configDirectory;
    QString privateKey;
    QString fileName;

    // Methoden
    QString getConfigDirectory ();
    QString getPrivateKey ();
    QString getCryptFileName ();

};

#endif // USERCONTROLL_H
