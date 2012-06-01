#ifndef CRYPT_H
#define CRYPT_H

#include <QtCore>

class Crypt
{
public:    
    Crypt();
    ~Crypt ();
    void setSecretKey (const QString &key);    

    QByteArray cryptPlainTextExt (const QByteArray &plain);
    QByteArray cryptToPlainTextExt (const QByteArray &crypt);

private:    
    QString secretKey;        
    QStringList magicsKey();
};

#endif // CRYPT_H
