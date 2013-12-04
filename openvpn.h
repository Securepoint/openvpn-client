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
#include "settings.h"
#include "qthreadexec.h"
#include "database.h"

class OpenVpn : public QObject
{
    Q_OBJECT

public:
    OpenVpn(Database *database);
    virtual ~OpenVpn() {}

    //Methoden
    QString getErrorString ();
    QString getConnectionIP ();
    QString getScript (const QString &type);
    QString getAdvName ();

    void setId (int id);

    bool hasCrediantials (int type = -1);
    QString getSavedUserData (int type);

    void runScript (const QString &type);
    void runDelayStartScript ();
    void setObjectToConnected ();
    void connectToVpn (bool openLog=false);
    void setConnectedIP (const QString &ip);
    void setIsConnected (bool flag);    
    void setIsError (bool flag);
    void setErrorString (const QString &errMes);
    void setIsConnecting (bool flag);    
    void setConnectedSinceTime (const QTime &since);
    void setConnectedSinceDate (const QDate &since);
    void enableAllMenus ();
    void disableMenues ();

    bool isConnecting ();
    bool isError ();
    bool isConnectionStable ();
    bool isOnConnect ();    
    void testCloseOpenVPN ();

    QAction *menu;

    int id() const;

    QTime getConnectedSinceTime ();
    QDate getConnectedSinceDate ();

    QString getConfigFullPath () const;
    QString getConfigPath () const;
    QString getConfigDirectory () const;
    QString getConfigName () const;
    bool isConnectionStable () const;

    void setConfigName (const QString &name);
    void setConfigPath (const QString &path);

    void setConfigStable (const bool &flag);

    QStringList makeProxyString ();    

    bool isStartConfig () const;
    void setStartConfig (bool flag);

    Database *db;

private slots:
    void showProcessError (QProcess::ProcessError error);
    void showProcessScriptError (QProcess::ProcessError error);
    void processFinished (int exitCode, QProcess::ExitStatus exitStatus);
    void readProcessData ();
    void startAfterConnectDelayed ();    
    void getCryptKey (QString key);

public slots:
    void openConnect ();
    void disconnectVpn ();
    void openVpnLog ();    
    void openEditConfig ();
    void openExport ();
    void openManageConnection ();
    void writeUserData (QString data);
    void saveUserData (int id, int type, QString value, bool save);
    void removeCredentials (bool refresh = true);

private:    
    // Methoden
    int _id;

    void setDisconnected ();
    void showTrayMessage (const QString &message);    

    bool onDisconnect;
    bool onConnect;
    bool errorHasOccurred;    

    QString errMessage;

    // Member
    QProcess proc;
    QProcess *procScripts;

    QList<QString> openVpnLogData;

    QSystemTrayIcon *appIcon;

    QList<QAction*> menuChildList;

    QString connectionIP;    

    QDate connectedSinceDate;
    QTime connectedSinceTime;

    // Public Member
    QString configName;
    QString configPath;
    QString configPwd;
    QString configUser;
    bool connectionStable;    
    bool runAsService;

    volatile bool waitForCryptKey;

    // Forms
    VpnLog mLog;

    void userDataIsNeeded (int type);

    bool delayed;
    bool isStartConfigValue;

signals:
    void configSignalIsChanged ();
    void errorOccuredSig (QString message);
    void connectionIsStableSig (QString ip);
    void connectionIsDisconnectedSig ();
    void connectionReconnectFromOpenVPNSig ();
    void needUserInput (int id, int type);
    void closeDialog ();
};

#endif // OPENVPN_H
