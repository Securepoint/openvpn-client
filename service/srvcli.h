#ifndef SRVCLI_H
#define SRVCLI_H

#include <QtCore>
#include <QtNetwork>
#include <QSslSocket>

class SrvCLI : public QObject
{
    Q_OBJECT
public:
    static SrvCLI *instance ();    
    void resetConnection ();
    bool isOnline () const;
    void setPort (const quint16 &clientPort);

public slots:
    void send (const QString &command, const QString &params=QString(""), bool fastmode = false);

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
    volatile bool work;

    quint16 port;

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
