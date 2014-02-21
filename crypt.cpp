#include "crypt.h"
#include "qca.h"

Crypt::Crypt()
    : secretKey("aes128-cbc-pkcs7-sn93-sh21-jks-12")
{        
}

Crypt::~Crypt() {

}

void Crypt::setSecretKey(const QString &key) {
    this->secretKey = key;
}

QStringList Crypt::magicsKey()
{
    QStringList lst = QString("aes128-cbc-pkcs7-sn93-sh21-jks-12").split(QLatin1String("-"));
    lst = lst.replaceInStrings(QLatin1String("c"), QLatin1String("m"));
    lst.removeAt(0);
    lst.removeAt(5);
    return lst;
}

QByteArray Crypt::cryptPlainTextExt (const QByteArray &plain)
{
    QByteArray res;

    QCA::Initializer init;
    QCA::SecureArray arg = plain;

    QByteArray key_arr;

    if (QCA::isSupported("aes128-cbc-pkcs7")) {

        key_arr = secretKey.toAscii();

        QCA::SymmetricKey key(key_arr);
        QCA::InitializationVector iv(QVariant(magicsKey().join("<")).toByteArray());

        QCA::Cipher cipher(QLatin1String("aes128"), QCA::Cipher::CBC, QCA::Cipher::DefaultPadding, QCA::Encode, key, iv);
        QCA::SecureArray u = cipher.update(arg);

        if (!cipher.ok()) {            
            return res;
        }

        QCA::SecureArray f = cipher.final();

        if (!cipher.ok()) {            
            return res;
        }

        res = QCA::arrayToHex(u.toByteArray()).toAscii() + QCA::arrayToHex(f.toByteArray()).toAscii();
    }
	else
	{
		MessageBoxA(NULL, "No support", "", 0);
	}

    return res;
}

QByteArray Crypt::cryptToPlainTextExt (const QByteArray &crypt)
{
    QByteArray res("");
    QByteArray key_arr;
    QCA::Initializer init;

    if (QCA::isSupported("aes128-cbc-pkcs7")) {

        key_arr = secretKey.toAscii();

        QCA::SymmetricKey key(key_arr);
        QCA::InitializationVector iv(QVariant(magicsKey().join("<")).toByteArray());

        QCA::Cipher cipher(QLatin1String("aes128"), QCA::Cipher::CBC, QCA::Cipher::DefaultPadding, QCA::Decode, key, iv);

        // Build a single cipher text array. You could also call update() with
        // each block as you receive it, if that is more useful.

        QCA::SecureArray arg = QCA::hexToArray(crypt);
        QCA::SecureArray cipherText = arg;//u.append(crypt);
        // take that cipher text, and decrypt it
        QCA::SecureArray u = cipher.update(cipherText);
        // check if the update() call worked
        if (!cipher.ok()) {
            return res;
        }

        // Again we need to call final(), to get the last block (with its padding removed)
        QCA::SecureArray f = cipher.final();
        // check if the final() call worked
        if (!cipher.ok()) {
            return res;
        }

        res = u.toByteArray() + f.toByteArray();
    }

    return res;
}
