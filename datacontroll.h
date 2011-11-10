#ifndef DATACONTROLL_H
#define DATACONTROLL_H

#include <QtCore/QCoreApplication>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtCore/QTextStream>
#include <QtCore/QDateTime>
#include <QtCore/QStringList>
#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <QtCore/QTimer>
#include <QDate>
#include <QTime>

#include "preferences.h"
#include "settings.h"
#include "mainwindowcontroll.h"
#include "treebutton.h"

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


class DataControll : public QTcpServer
{
    Q_OBJECT
public:
    DataControll(quint16 port, QObject* parent = 0);
    void incomingConnection(int socket);
    void sendConnect ();
    void sendUser ();
    void sendPwd ();
    void sendPrivKey ();
    void sendDisconnect ();
    void sendDisconnectWithoutGUI ();
    void sendGetStatus ();
    void sendChallengeKey ();
    bool isConnectedService;
    QString config;
    QString configPath;
    QString configName;
    QString configUser;
    QString configPwd;
    quint16 servicePort;
    bool autoReconnect;
    bool onConnect;
    void openConnectionToService (bool showlog);
    void runScript (QString type);
    void setTreeButton (TreeButton *but);
    TreeButton *getTreeButton ();

private:
    QString vpnName;
    QTimer *timer;
    QString getScript (QString type);

    QProcess *procScripts;

    void serviceNotAvailable ();
    bool userDataLock;
    bool scScriptRun;
    TreeButton *button;
    bool disconnectSend;

private slots:
    void makeDisconnectSingleShoot ();

public slots:
    void discardClient();
    void readClient();
    void refreshStatus ();
    void startAfterConnectDelayed ();
    void restartConnection ();
    void userDataIsAvailable ();
};

#endif // DATACONTROLL_H
