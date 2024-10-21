#ifndef SRVCLI_H
#define SRVCLI_H

#include <QtCore>
#include <QtNetwork>

#include "sslserverconnection.h"

class SrvCLI : public QObject
{
    Q_OBJECT
public:
    static SrvCLI *instance ();
    bool send (const QString &command, const QString &params=QString(""));
    bool sendPass (const QString &command, const QString &params=QString(""));
    void resetConnection ();
    bool isOnline () const;
    bool makeFastConnection ();
    void closeConnection();
private:
    static SrvCLI *mInst;
    SrvCLI();

    QSslSocket sslSocket;
    QTime startTime;
    QString response;
    bool connectionIsOnline;
    qint64 blockSize;

private slots:
    void sendRequest (const QString &command, const QString &params=QString(""));
    void sendRequestPass (const QString &command, const QString &params=QString(""));
    void stopRequest ();
    void connectionClosedByServer ();
    void error ();
    void slot_peerVerifyError (const QSslError &error);
    void slot_sslErrors (const QList<QSslError> &errors);
    void slot_stateChanged (QAbstractSocket::SocketState state);

    void slotStartRead();
    void slotConnectionClosed();
    void slotError(QAbstractSocket::SocketError err);

signals:
    void connectionIsClosed ();
    void sendMessage (const QString &message, const QString &command=QString("LOG"));
    void setDisconnected (int id);
    void setError (int id, QString message);
    void needUserInput (int id, int type);
    void receivedDummy (quint32);
    void receivedIP (int id, QString ip);
    void receivedReconnect (int id);
    void receivedTapControl (int type);
    void receivedRemoveTap (QString state);
    void receivedStatus (int id, bool isConnected, bool isConnecting, int lastAction, QString ip);
    void receivedTapCount(int count);
    void getVpnLog(int id, SslServerConnection * sender);
};

#endif // SRVCLI_H
