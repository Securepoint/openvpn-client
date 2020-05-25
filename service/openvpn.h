#ifndef OPENVPN_H
#define OPENVPN_H

#include <QtCore/QDir>
#include <QtCore/QTimer>
#include <QtCore/QProcess>
#include <QtCore/QTextStream>
#include <QtNetwork/QTcpSocket>
#include <QtCore/QDateTime>
#include <QtCore/QCoreApplication>

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSystemTrayIcon>
#include <QtCore/QEvent>
#include <QtGui/QShowEvent>
#include <QtGui/QMovie>
#include <QtCore/qstack.h>
#include <QtCore/QDateTime>
#include <QtCore/QDir>

#include "srvcli.h"

class OpenVpn : public QObject
{
    Q_OBJECT

public:
    OpenVpn(SrvCLI * srvCLI);
    virtual ~OpenVpn();

    void connectToVpn ();
    void sendStatus ();
    void setUsername (const QString &username);
    void setPassword (const QString &pwd);
    void setPrivateKey (const QString &key);
    void setChallengeKey (const QString &key);
    void setUseInteract (const QString &interact);
    void setConfigName (const QString &name);
    void setConfigPath (const QString &path);
    void setProxyString (const QString &proxy);
    bool isConnectionStable () const;
    bool isConnecting () const;
    int id () const;

public slots:
    void setId (const int &ident);
    void disconnectVpn (bool silen = false);

private:
    volatile bool onDisconnect;
    bool useNoInteract;
    volatile bool connectionStable;
    volatile bool connecting;

    int _id;
    int retryMultiHostCount;
    int lastAction;
    QString configName;
    QString configPath;
    QString configPrivKey;
    QString challengeKey;

    QString connectionIP;
    QString connectionLineBak;

    QProcess proc;
    QProcess *procScripts;

    QString proxyString;
    QString challangeUserName;

    SrvCLI * _srvCLI;

    bool connectionIsAOtpConnection;

private slots:
    void showProcessError (QProcess::ProcessError error);
    void readProcessData ();
    void processFinished (int exitCode, QProcess::ExitStatus exitStatus);
    void errorSocket (QAbstractSocket::SocketError err);

signals:
    void removeItemFromList (int id);
    void foobar (int id);
};

#endif // OPENVPN_H
