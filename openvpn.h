#ifndef OPENVPN_H
#define OPENVPN_H

#include <QtGui>
#include <QDir>
#include <QDialog>
#include <QSystemTrayIcon>
#include "StatusMessage.h"
#include "vpnLog.h"
// Prevent Circle References
class UserAuth;
#include "userauth.h"



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



private slots:
    void showProcessError (QProcess::ProcessError error);
    void processFinished (int stat, QProcess::ExitStatus status);
    void processIsStarted ();
    void readProcessData ();

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
    QString connectionIP;
    QStringList openVpnLogData;
    QSystemTrayIcon *appIcon;
    QList<QAction*> menuList;
    QList<QAction*> menuChildList;
    // Forms
    StatusMessage mCon;
    VpnLog mLog;
    UserAuth *userauth;

signals:
    void configSignalIsChanged ();
};

#endif // OPENVPN_H
