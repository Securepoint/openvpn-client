#ifndef SSLSERVERCONNECTION_H
#define SSLSERVERCONNECTION_H

#include <QtCore>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QSslSocket>

#include "sslkeycontent.h"
#include "paths.h"

class SslServerConnection : public QThread
{
    Q_OBJECT
public:
    SslServerConnection(QObject *parent = 0);
    SslServerConnection(quint16 socket, QObject *parent = 0);
    ~SslServerConnection();

    const quint64 threadId () const;
    void run ();

public slots:
    void slotStartRead();
    void slotConnectionClosed();
    void slotError(QAbstractSocket::SocketError err);

private:
    qint64 blockSize;
    QSslSocket *socket;

    static quint64 internalId;

signals:
    void connectionIsClosed ();
    void sendMessage (const QString &message, const QString &command=QString("LOG"));
    void setDisconnected (int id);
    void setError (int id, QString message);
    void needUserInput (int id, int type);
    void receivedDummy ();
    void receivedIP (int id, QString ip);
    void receivedReconnect (int id);
    void receivedTapControl (int type);
    void receivedRemoveTap (QString state);
    void receivedStatus (int id, bool isConnected, bool isConnecting, int lastAction, QString ip);
    void receivedTapCount(int count);
    void getVpnLog(int id, SslServerConnection * sender);

};

#endif // SSLSERVERCONNECTION_H
