#ifndef CRYPT_H
#define CRYPT_H

#include <QtCore>

// Singleton

class Crypt
{
public:
    static Crypt *getInstance ();
    void setSecretKey (QString key);
    QString cryptPlainText (const QString &plain);
    QString cryptToPlainText (const QString &cyrpt);

private:
    static Crypt *mInst;
    Crypt();
    QString secretKey;
};

#endif // CRYPT_H
