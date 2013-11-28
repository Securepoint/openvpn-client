#ifndef OPENVPN_H
#define OPENVPN_H

#include <QtCore/QDir>
#include <QtCore/QTimer>
#include <QtCore/QProcess>
#include <QtCore/QTextStream>
#include <QtNetwork/QTcpSocket>
#include <QtCore/QDateTime>
#include <QtCore/QCoreApplication>

#include <QtCore>

class OpenVpn : public QObject
{
    Q_OBJECT

public:
    OpenVpn();
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
    void disconnectVpn ();

private:
    volatile bool onDisconnect;
    bool useNoInteract;
    volatile bool connectionStable;
    volatile bool connecting;

    int _id;
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
