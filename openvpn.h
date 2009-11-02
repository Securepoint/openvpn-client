#ifndef OPENVPN_H
#define OPENVPN_H

#include <QtGui>
#include <QDir>
#include <QDialog>
#include <QSystemTrayIcon>
#include <QTimer>
#include "editconfig.h"
#include "vpnlog.h"
// Prevent Circle References
class StdUserAuth;
#include "stduserauth.h"

#include "pkcs_auth.h"


class OpenVpn : public QObject
{
    Q_OBJECT

public:
    OpenVpn();
    virtual ~OpenVpn() {}

    //Methoden
    void connectToVpn (bool openLog=false);
    void setTray (QSystemTrayIcon *appIcon);
    void setMenuList (QList<QAction*> List);
    void setMenuChildList (QList<QAction*> List);
    QString getScript (QString type);
    void runScript (QString type);
    void setObjectToConnected ();
    bool isConnectionStable ();
    // Public Member
    QString configName;
    QString configPath;
    QString configPwd;
    QString configUser;
    bool connectionStable;

private slots:
    void showProcessError (QProcess::ProcessError error);
    void showProcessScriptError (QProcess::ProcessError error);
    void processFinished (int stat, QProcess::ExitStatus status);
    void processIsStarted ();
    void readProcessData ();
    void startAfterConnectDelayed ();

public slots:
    void openConnect ();
    void disconnectVpn ();
    void editVpnConfig ();
    void openVpnLog ();
    void configIsChanged ();

private:
    // Enum für die Icons
    enum Icons {
        Inaktiv,
        Connected,
        Error,
        Connecting
    };
    // Methoden
    void setConnected ();
    void setDisconnected ();
    bool onDisconnect;
    void showTrayMessage (QString message);
    void setIcon(int index);
    QStringList makeProxyString ();

    // Member
    QProcess *proc;
    QProcess *procScripts;
    QString connectionIP;
    QList<QString> openVpnLogData;
    QSystemTrayIcon *appIcon;
    QList<QAction*> menuList;
    QList<QAction*> menuChildList;
    // Forms
    EditConfig mCon;
    VpnLog mLog;
    StdUserAuth *userauth;
    pkcs_auth pkcsAuth;

signals:
    void configSignalIsChanged ();
};

#endif // OPENVPN_H
