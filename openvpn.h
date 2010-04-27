#ifndef OPENVPN_H
#define OPENVPN_H

#include <QtGui>
#include <QDir>
#include <QDialog>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QTime>
#include <QDate>

#include "vpnlog.h"
#include "mainwindowcontroll.h"
#include "settings.h"
#include "usercontroll.h"
#include "editconfig.h"
#include "configexport.h"

// Prevent Circle References
#include "frmgetuserdata.h"


class OpenVpn : public QObject
{
    Q_OBJECT

public:
    OpenVpn();
    virtual ~OpenVpn() {}

    //Methoden
    QString getErrorString ();
    QString getConnectionIP ();
    QString getScript (QString type);
    QString getAdvName ();

    void runScript (QString type);
    void setObjectToConnected ();
    void connectToVpn (bool openLog=false);
    void setConnectedIP (QString ip);
    void setIsConnected (bool flag);
    void setIsError (bool flag);
    void setErrorString (QString errMes);
    void setIsConnecting (bool flag);
    void setAdvName (QString name);
    void setIsCryptFileAvailable (bool flag);
    void setConnectedSinceTime (QTime since);
    void setConnectedSinceDate (QDate since);
    void enableAllMenus ();
    void disableMenues ();

    bool isConnecting ();
    bool isError ();
    bool isConnectionStable ();
    bool isOnConnect ();
    bool getIsCryptFileAvailable ();
    void testCloseOpenVPN ();

    QTime getConnectedSinceTime ();
    QDate getConnectedSinceDate ();


    // Public Member
    QString configName;
    QString configPath;
    QString configPwd;
    QString configUser;
    QAction *menu;
    bool connectionStable;
    bool isLinked;
    bool runAsService;


private slots:
    void showProcessError (QProcess::ProcessError error);
    void showProcessScriptError (QProcess::ProcessError error);
    void processFinished (int exitCode, QProcess::ExitStatus exitStatus);
    void readProcessData ();
    void startAfterConnectDelayed ();
    void userDataIsAvailable ();

public slots:
    void openConnect ();
    void disconnectVpn ();
    void openVpnLog ();
    void configIsChanged ();
    void openEditConfig ();
    void openExport ();

private:
    // Enum für die Icons
    enum Icons {
        Inaktiv,
        Connected,
        Error,
        Connecting
    };
    // Methoden

    void setDisconnected ();
    void showTrayMessage (QString message);
    void setIcon(int index);

    bool onDisconnect;
    bool onConnect;
    bool errorHasOccurred;

    QString errMessage;
    QStringList makeProxyString ();

    // Member
    QProcess *proc;
    QProcess *procScripts;

    QList<QString> openVpnLogData;

    QSystemTrayIcon *appIcon;

    QList<QAction*> menuChildList;

    QString connectionIP;
    QString advName;

    bool isCryptFileAvailable;
    bool isUserDataAvailable;

    QDate connectedSinceDate;
    QTime connectedSinceTime;



    // Forms
    VpnLog mLog;

signals:
    void configSignalIsChanged ();
    void errorOccuredSig (QString message);
    void connectionIsStableSig (QString ip);
    void connectionIsDisconnectedSig ();
    void connectionReconnectFromOpenVPNSig ();
};

#endif // OPENVPN_H
