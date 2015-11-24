#ifndef SSLKEYCONTENT_H
#define SSLKEYCONTENT_H

#include <QtCore>
#include "paths.h"

class SslKeyContent
{
public:
    static SslKeyContent *instance ();
    const QByteArray getKey () const;
    const bool keyIsValid () const;

private:
    SslKeyContent();
    static SslKeyContent *mInst;
    QByteArray keyContent;
};

#endif // SSLKEYCONTENT_H
