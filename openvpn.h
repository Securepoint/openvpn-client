#ifndef OPENVPN_H
#define OPENVPN_H

#include <QtGui>
#include <QDir>
#include <QDialog>
#include <QSystemTrayIcon>
#include <QTimer>
#include "StatusMessage.h"
#include "vpnLog.h"
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
    void connectToVpn ();
    void setTray (QSystemTrayIcon *appIcon);
    void setMenuList (QList<QAction*> List);
    void setMenuChildList (QList<QAction*> List);
    // Public Member
    QString configName;
    QString configPath;
    QString configPwd;
    QString configUser;
    bool isConnectionStable ();
    QString getScript (QString type);
    void runScript (QString type);

private slots:
    void showProcessError (QProcess::ProcessError error);
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
    bool connectionStable;
    bool onDisconnect;
    void showTrayMessage (QString message);
    void setIcon(int index);
    // Member
    QProcess *proc;
    QProcess *procScripts;
    QString connectionIP;
    QStringList openVpnLogData;
    QSystemTrayIcon *appIcon;
    QList<QAction*> menuList;
    QList<QAction*> menuChildList;
    // Forms
    StatusMessage mCon;
    VpnLog mLog;
    StdUserAuth *userauth;
    pkcs_auth pkcsAuth;

signals:
    void configSignalIsChanged ();
};

#endif // OPENVPN_H
