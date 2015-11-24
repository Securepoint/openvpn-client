#ifndef CRYPT_H
#define CRYPT_H

#include <QtCore>

class Crypt
{
public:
    static void setSecretKey (const QString &key);

    static QString encodePlaintext (const QString &plain);
    static QString encodePlaintext (const QByteArray &plain);
    static QByteArray decodeToPlaintext (const QString &crypt);
    static QString secretKey;
    static bool exceptionEnabled;
private:
    Crypt();
    ~Crypt ();
};

#endif // CRYPT_H
