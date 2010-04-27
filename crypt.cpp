#include "crypt.h"

Crypt *Crypt::mInst = NULL;

Crypt *Crypt::getInstance() {
    if (!mInst)
        mInst = new Crypt ();
    return mInst;
}

Crypt::Crypt()
{
    this->secretKey = "";
}

void Crypt::setSecretKey(QString key) {
    this->secretKey = key;
}

QString Crypt::cryptPlainText(const QString &plain) {
    QString encText = "";

    char buffer[3];

    int pos = plain.length() % this->secretKey.length();

    for (int i=0; i<plain.length(); i++) {
        sprintf(buffer, "%03d", (unsigned char)plain[i].toAscii() ^ this->secretKey[(i+pos) % this->secretKey.length()].toAscii());
        encText = encText + QString(buffer);
    }
    return encText;
}

QString Crypt::cryptToPlainText(const QString &cyrpt) {
    QString plainText = "";
    QChar c;
    char digit;

    int pos = (cyrpt.length() / 3 % this->secretKey.length());

    for (int i=0; i<cyrpt.length()/3; i++) {
        int number = 0;
        c = cyrpt[i*3]; digit = c.toAscii();   if (digit<'0' || digit>'9') return "";
        number += (digit-'0') * 100;
        c = cyrpt[i*3+1]; digit = c.toAscii(); if (digit<'0' || digit>'9') return "";
        number += (digit-'0') * 10;
        c = cyrpt[i*3+2]; digit = c.toAscii(); if (digit<'0' || digit>'9') return "";
        number += digit - '0';
        plainText = plainText + QChar(number ^ this->secretKey[(i + pos) % this->secretKey.length()].toAscii());
    }
    return plainText;
}
