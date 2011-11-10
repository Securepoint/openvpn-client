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

class Log {
public:
    static void write (const QString &message) {
        QFile log ("c:/newsrvlog.log");
        if (log.open(QIODevice::WriteOnly | QIODevice::Append)) {
            QTextStream out (&log);
            out << message << "\n";
            log.waitForBytesWritten(300);
            log.close();
        }
    }
};

class OpenVpn : public QObject
{
    Q_OBJECT

public:
    OpenVpn();
    virtual ~OpenVpn() {}

    //Methoden
    void connectToVpn ();
    bool isConnectionStable ();
    void sendStatus ();
    // Public Member
    QString configName;
    QString configPath;
    QString configPwd;
    QString configUser;
    QString configPrivKey;
    QString challengeKey;
    bool waitForData;
    quint16 clientPort;
    QCoreApplication *capp;
    QString appPath;
    void setUsername (QString username);
    void setPWd (QString pwd);
    void setPrivKey (QString key);
    void setChallengeKey (QString key);
    bool useNoInteract;


private slots:
    void showProcessError (QProcess::ProcessError error);
    void readProcessData ();    
    void processFinished (int exitCode, QProcess::ExitStatus exitStatus);
    void errorSocket (QAbstractSocket::SocketError err);

public slots:
    void disconnectVpn ();

private:
    // Methoden
    bool onDisconnect;
    void writeLogDataToSocket(QString logData);

    // Member
    QProcess *proc;
    QProcess *procScripts;
    QString connectionIP;
    QString connectionLineBak;
    bool connectionStable;
    bool isLinked;
};

#endif // OPENVPN_H
