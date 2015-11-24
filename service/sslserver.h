/*
Logserver  Project

*/

#ifndef SSLSERVER_H
#define SSLSERVER_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSystemTrayIcon>
#include <QtCore/QEvent>
#include <QtGui/QShowEvent>
#include <QtGui/QMovie>
#include <QtCore/qstack.h>
#include <QtCore/QDateTime>
#include <QtCore/QDir>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QSslSocket>

#include "sslconnections.h"

class OpenVpn;

class SslServer : public QTcpServer
{
    Q_OBJECT
public:
    SslServer(quint16 port, QObject *parent = 0);
    ~SslServer ();
    SslConnections *connectionsList () const;

private:
    void incomingConnection (int socketDescriptor);
    SslConnections *connections;    

private slots:
    void slotThreadFinished ();
    void slotQuitThread ();
    void OnStopService();
};

#endif // SSLSERVER_H
