#ifndef SSLCONNECTIONS_H
#define SSLCONNECTIONS_H

#include <QtCore>
#include "sslserverconnection.h"

class SslConnections
{
public:
    SslConnections();

    SslServerConnection *element (const int &index = 0) const;
    int count () const;
    int indexOf (SslServerConnection *element) const;
    void remove (const quint64 &id);
    void add (SslServerConnection *element);
    void clear ();

private:
    QList<SslServerConnection *> connectionList;
};

#endif // SSLCONNECTIONS_H
