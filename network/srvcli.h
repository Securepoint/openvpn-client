#ifndef SRVCLI_H
#define SRVCLI_H

#include <QtCore>
#include <QtNetwork>
#include "transactionthread.h"

class SrvCLI : public QObject
{
    Q_OBJECT
public:
    static SrvCLI *instance ();
    void send (const QString &command, const QString &params=QString(""), bool fastmode = false);
    void resetConnection ();
    bool isOnline () const;

private:
    static SrvCLI *mInst;
    SrvCLI();
    void closeConnection ();
    QSslSocket sslSocket;
    quint64 nextBlockSize;
    QTime startTime;    
    QString response;
    bool connectionIsOnline;
    bool makeConnection (bool fastmode);

private slots:
    void sendRequest (const QString &command, const QString &params=QString(""));
    void stopRequest ();
    void connectionClosedByServer ();
    void error ();
    void slot_peerVerifyError (const QSslError &error);
    void slot_sslErrors (const QList<QSslError> &errors);
    void slot_stateChanged (QAbstractSocket::SocketState state);
};

#endif // SRVCLI_H
