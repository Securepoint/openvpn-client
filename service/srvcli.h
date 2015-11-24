#ifndef SRVCLI_H
#define SRVCLI_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSystemTrayIcon>
#include <QtCore/QEvent>
#include <QtGui/QShowEvent>
#include <QtGui/QMovie>
#include <QtCore/qstack.h>
#include <QtCore/QDateTime>
#include <QtCore/QDir>
#include <QtNetwork\qsslsocket.h>

class SrvCLI : public QObject
{
    Q_OBJECT
public:   
    SrvCLI(QSslSocket * socket);
    void resetConnection ();
    bool isOnline () const;
    void setPort (const quint16 &clientPort);

public slots:
    void send (const QString &command, const QString &params=QString(""), bool fastmode = false);

private:
    
    void closeConnection ();
    QSslSocket* sslSocket;
    quint64 nextBlockSize;
    QTime startTime;    
    QString response;
    bool connectionIsOnline;
    volatile bool work;

    quint16 port;

private slots:
    void sendRequest (const QString &command, const QString &params=QString(""));
    void stopRequest ();
};

#endif // SRVCLI_H
