#ifndef CRYPT_H
#define CRYPT_H

#include <QtCore>

class DPAPI
{
public:
    static QByteArray encodePlaintextUtf8(const QByteArray &inputUtf8);
    static QByteArray decodeToPlaintext(const QByteArray &inputHex);

private:
    DPAPI(){}
};

class Crypt
{
public:
    static void enableDPAPI ();
    static bool isDPAPIEnabled ();
    static void setSecretKey (const QString &key);
    static void clearEncryptedArray();

    // Bridge dpapi or old one
    static QString encodePlaintext (const QString &plain);
    // Old one
    static QString encodePlaintextUtf8 (const QByteArray &plain);
    // Old one
    static QString encodePlaintextAscii (const QByteArray &plain);
    // Bridge dpapi or old one
    static QByteArray decodeToPlaintext (const QString &crypt);
    // Old one
    static QByteArray decodeToPlaintextWithKey (const QString &crypt);

    static QString secretKey;
    static bool exceptionEnabled;
    static QByteArray encrypted;

private:
    Crypt();
    ~Crypt ();
    static bool useDPAPI;
};

#endif // CRYPT_H
