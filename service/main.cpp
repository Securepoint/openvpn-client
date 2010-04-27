#include <QtCore/QCoreApplication>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtCore/QTextStream>
#include <QtCore/QDateTime>
#include <QtCore/QStringList>
#include <QtCore/QDir>
#include <QtCore/QSettings>

#include "qtservice.h"
#include "openvpn.h"

class OpenVPNDaemon : public QTcpServer
{
    Q_OBJECT
public:
    OpenVPNDaemon(quint16 port, quint16 cport, QString appPath, QObject* parent = 0)
        : QTcpServer(parent), disabled(false)
    {
        listen(QHostAddress::LocalHost, port);
        this->vpnObj = new OpenVpn();
        this->clport = cport;
        this->appStartPath = appPath;
        this->vpnObj->clientPort = this->clport;
        this->vpnObj->appPath = this->appStartPath;
        QCoreApplication *capp = dynamic_cast<QCoreApplication*>(this->parent());
        this->vpnObj->capp = capp;
    }

    void incomingConnection(int socket)
    {
        if (disabled)
            return;

        QTcpSocket* s = new QTcpSocket(this);
        connect(s, SIGNAL(readyRead()), this, SLOT(readClient()));
        connect(s, SIGNAL(disconnected()), this, SLOT(discardClient()));
        s->setSocketDescriptor(socket);


    }

    void pause()
    {
        disabled = true;
    }

    void resume()
    {
        disabled = false;
    }

private:
    quint16 clport;
    QString appStartPath;
    void writeLogDataToSocket(QString logData){
        QTcpSocket *socket = new QTcpSocket(this);

        socket->connectToHost(QString("127.0.0.1"), this->clport);

        if (socket->waitForConnected(500)) {
            QTextStream os(socket);

            os.setAutoDetectUnicode(true);
            os << "log->" + logData.replace("\r\n", "#<>#") + "\r\n"
               << QDateTime::currentDateTime().toString() << "\n";

            socket->disconnectFromHost();
        }
        if (socket->state() == QAbstractSocket::UnconnectedState ||
                               socket->waitForDisconnected(500)) {
            delete socket;
        }
    }

private slots:
    void readClient()
    {        
        if (disabled)
            return;
        QString command = "";
        QString config = "";
        QString configUser = "";
        QString configPwd = "";
        QString configPrivKey = "";
        QString challengeKey = "";
        bool useNoInteract = false;

        QTcpSocket* socket = (QTcpSocket*)sender();
        if (socket->canReadLine()) {
            // Get Command from the client
            QStringList tokens = QString(socket->readAll()).split(QRegExp("[\r\n][\r\n]*"));

            for (int i=0; i<tokens.size();i++) {
                if (QString(tokens[i]).indexOf("command->") != -1) {
                    // Command found
                    if (QString(tokens[i]).split("->").size() != 2) {
                        // Wrong format
                        this->writeLogDataToSocket(QString("Wrong format command"));
                    } else {
                        QStringList spl = QString(tokens[i]).split("->");
                        command = spl[1];
                    }
                } else if (QString(tokens[i]).indexOf("config->") != -1) {
                    // Config found
                    if (QString(tokens[i]).split("->").size() != 2) {
                        // Wrong format
                        this->writeLogDataToSocket(QString("Wrong format config"));
                    } else {
                        QStringList spl = QString(tokens[i]).split("->");
                        config = spl[1];
                    }
                } else if (QString(tokens[i]).indexOf("usenointact->") != -1) {
                    // Config found
                    if (QString(tokens[i]).split("->").size() != 2) {
                        // Wrong format
                        this->writeLogDataToSocket(QString("Wrong format no interact"));
                    } else {
                        QStringList spl = QString(tokens[i]).split("->");
                        useNoInteract = (QString(spl[1]).trimmed() == "1" ? true : false);
                    }
                } else if (QString(tokens[i]).indexOf("user->") != -1) {
                    // Config found
                    if (QString(tokens[i]).split("->").size() != 2) {
                        // Wrong format
                        this->writeLogDataToSocket(QString("Wrong format user"));
                    } else {
                        QStringList spl = QString(tokens[i]).split("->");
                        configUser = spl[1];
                    }
                } else if (QString(tokens[i]).indexOf("pwd->") != -1) {
                    // Config found
                    if (QString(tokens[i]).split("->").size() != 2) {
                        // Wrong format
                        this->writeLogDataToSocket(QString("Wrong format pwd"));
                    } else {
                        QStringList spl = QString(tokens[i]).split("->");
                        configPwd = spl[1];
                    }
                } else if (QString(tokens[i]).indexOf("uneeded->") != -1) {
                    // Config found
                    if (QString(tokens[i]).split("->").size() != 2) {
                        // Wrong format
                        this->writeLogDataToSocket(QString("Wrong format uneeded"));
                    } else {
                        QStringList spl = QString(tokens[i]).split("->");
                        command = "uneeded";
                        configUser = spl[1];
                    }
                } else if (QString(tokens[i]).indexOf("pneeded->") != -1) {
                    // Config found
                    if (QString(tokens[i]).split("->").size() != 2) {
                        // Wrong format
                        this->writeLogDataToSocket(QString("Wrong format pneeded"));
                    } else {
                        QStringList spl = QString(tokens[i]).split("->");
                        command = "pneeded";
                        configPwd = spl[1];
                    }
                } else if (QString(tokens[i]).indexOf("pkneeded->") != -1) {
                    // Config found
                    if (QString(tokens[i]).split("->").size() != 2) {
                        // Wrong format
                        this->writeLogDataToSocket(QString("Wrong format pkneeded"));
                    } else {
                        QStringList spl = QString(tokens[i]).split("->");
                        command = "pkneeded";
                        configPrivKey = spl[1];
                    }
                } else if (QString(tokens[i]).indexOf("ckneeded->") != -1) {
                    // Config found
                    if (QString(tokens[i]).split("->").size() != 2) {
                        // Wrong format
                        this->writeLogDataToSocket(QString("Wrong format ckneeded"));
                    } else {
                        QStringList spl = QString(tokens[i]).split("->");
                        command = "ckneeded";
                        challengeKey = spl[1];
                    }
                } else if (QString(tokens[i]).indexOf("getstatus->") != -1) {
                    // Config found
                    command = "getstatus";
                }
            }

            if (command != "") {
                if (command == "open") {
                    if (config != "") {
                        QString configPath = config.trimmed().left(config.trimmed().lastIndexOf("/"));
                        QString configName = config.trimmed().right(config.trimmed().size() - config.trimmed().lastIndexOf("/") -1);
                        configName = configName.left(configName.size()-5);
                        // Make OpenVPN object
                        this->writeLogDataToSocket(QString("Try to start OpenVPN connection %1").arg(configName));
                        this->vpnObj->configName = configName;
                        this->vpnObj->configPath = configPath;
                        this->vpnObj->configUser = configUser;
                        this->vpnObj->configPwd = configPwd;
                        this->vpnObj->useNoInteract = useNoInteract;
                        this->vpnObj->connectToVpn();
                    }
                } else if (command == "close"){
                    this->vpnObj->disconnectVpn();
                } else if (command == "getstatus") {
                    this->vpnObj->sendStatus();
                } else if (command == "uneeded") {
                    this->vpnObj->setUsername(configUser);;
                } else if (command == "pneeded") {
                    this->vpnObj->setPWd(configPwd);;
                } else if (command == "pkneeded") {
                    this->vpnObj->setPrivKey(configPrivKey);;
                } else if (command == "ckneeded") {
                    this->vpnObj->setChallengeKey(challengeKey);
                }
            }
            // After execute command close the socket
            socket->close();
            QCoreApplication *capp = dynamic_cast<QCoreApplication*>(this->parent());
            while (socket->state() != QTcpSocket::UnconnectedState) {
                // wait for socket close
                capp->processEvents();
            }
            if (socket->state() == QTcpSocket::UnconnectedState) {
                delete socket;
            }
        }
    }

    void discardClient()
    {
        QTcpSocket* socket = (QTcpSocket*)sender();
        socket->deleteLater();
    }

private:
    bool disabled;
    OpenVpn *vpnObj;
};

class OpenVPNService : public QtService<QCoreApplication>
{
public:
    OpenVPNService(int argc, char **argv)
        : QtService<QCoreApplication>(argc, argv, "Securepoint VPN")
    {
        setServiceDescription("OpenVPN wrapper to connect to an OpenVPN server without administrator permission");
        setServiceFlags(QtServiceBase::CanBeSuspended);
    }

protected:
    void start()
    {
        QCoreApplication *app = application();

        quint16 port = 3656;
        quint16 cport = 3655;

        QFile settings(app->applicationDirPath() + "/settings.dat");
        if (settings.exists())
            if (settings.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream sin (&settings);
                while (!sin.atEnd()){
                    QString line = sin.readLine();
                    if (line.left(5) == "PORT:") {
                        port = (quint16) line.right(line.size()-5).toInt();
                    } else if (line.left(6) == "CPORT:") {
                        cport = (quint16) line.right(line.size()-5).toInt();
                    }
                }
                settings.close();
            }


        daemon = new OpenVPNDaemon(port, cport, app->applicationDirPath(), app);

        if (!daemon->isListening()) {
            logMessage(QString("Failed to bind to port %1").arg(daemon->serverPort()), QtServiceBase::Error);
            app->quit();
        }
        QtServiceBase::instance()->logMessage(QString("Service has been started on port %1.").arg(port));
    }

    void pause()
    {
	daemon->pause();
    }

    void resume()
    {
	daemon->resume();
    }

private:
    OpenVPNDaemon *daemon;
};

#include "main.moc"

int main(int argc, char **argv)
{
#if !defined(Q_WS_WIN)
    // QtService stores service settings in SystemScope, which normally require root privileges.
    // To allow testing this example as non-root, we change the directory of the SystemScope settings file.
    QSettings::setPath(QSettings::NativeFormat, QSettings::SystemScope, QDir::tempPath());
    qWarning("(Example uses dummy settings file: %s/QtSoftware.conf)", QDir::tempPath().toLatin1().constData());
#endif
    OpenVPNService service(argc, argv);
    return service.exec();
}
