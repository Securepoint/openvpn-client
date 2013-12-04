#ifndef SSLSERVERCONNECTION_H
#define SSLSERVERCONNECTION_H

#include <QtCore>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QSslSocket>

#include "sslkeycontent.h"
#include "paths.h"

class OpenVpn;

class SslServerConnection : public QThread
{
    Q_OBJECT
public:
    SslServerConnection(quint16 socket, QObject *parent = 0);
    ~SslServerConnection();

    const quint64 threadId () const;
    void run ();

public slots:
    void slotAcceptedClient();
    void slotStartRead();
    void slotConnectionClosed();
    void slotError(QAbstractSocket::SocketError err);
    void slotModeChanged (QSslSocket::SslMode mode);
    void removeItemFromList (int id);
    void recDig (int id);

private:
    qint64 blockSize;
    QSslSocket *socket;

    static quint64 internalId;    

    static QList<QPair<int, OpenVpn*> > vpnList;
    OpenVpn *foundItemInList (const int &id);

signals:
    void connectionIsClosed ();
};

#endif // SSLSERVERCONNECTION_H
