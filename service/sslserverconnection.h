#ifndef SSLSERVERCONNECTION_H
#define SSLSERVERCONNECTION_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSystemTrayIcon>
#include <QtCore/QEvent>
#include <QtGui/QShowEvent>
#include <QtGui/QMovie>
#include <QtCore/qstack.h>
#include <QtCore/QDateTime>
#include <QtCore/QDir>
#include <QtCore\qthread.h>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QSslSocket>

#include "srvcli.h"

class OpenVpn;

class SslServerConnection : public QThread
{
    Q_OBJECT
public:
    SslServerConnection(quint16 socket, QObject *parent = 0);
    ~SslServerConnection();

    const quint64 threadId () const;
    void run ();
    void setId(quint32 id);
    quint32 getId();
    void setName(QString name);
    QString getName();
    OpenVpn *foundItemInList (const int &id);
    SrvCLI * srvcli()
    {
        return this->_srvCLI;
    }

public slots:
    void slotAcceptedClient();
    void slotStartRead();
    void slotConnectionClosed();
    void slotError(QAbstractSocket::SocketError err);
    void removeItemFromList (int id);
    void recDig (int id);

private:
    qint64 blockSize;
    QSslSocket *socket;
    quint32 id;
    quint64 _threadId;
    QString name;

    static quint64 internalId;    

    SrvCLI * _srvCLI;

    QList<QPair<int, OpenVpn*>> vpnList;


signals:
    void connectionIsClosed ();
};

#endif // SSLSERVERCONNECTION_H
