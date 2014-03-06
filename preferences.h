#ifndef PREFERENCES_H
#define PREFERENCES_H

// Qt Includes
#include <QtCore>
#include <QtGui>

#include "database.h"
#include "update/parsexml.h"

class OpenVpn;
class TreeButton;
class SslServer;

QT_BEGIN_NAMESPACE
class QAction;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMenu;
class QPushButton;
class QSpinBox;
class QTextEdit;
QT_END_NAMESPACE

namespace Ui {
    class Preferences;
}

typedef QPair <int, OpenVpn*> ListObject;

class Preferences : public QDialog {
    Q_OBJECT
public:
    static Preferences *instance ();
    ~Preferences();
    void openDialog ();
    void setVisible(bool visible);
    void refreshConfigList ();
    void setIcon ();
    void showTrayMessage(const QString &message, QSystemTrayIcon::MessageIcon messageType, int duration=3000);
    void showTrayMessageChecked(const QString &message, QSystemTrayIcon::MessageIcon messageType, int duration=3000);
    QStringList vpnLog;
    QSystemTrayIcon *getSystrayIcon () const;
    bool startDaemon ();
    void searchStartConfigDir ();
    void refreshDialog ();
    void setConnectionStatus ();
    bool isConnectionActive () const;
    void closeAllOpenConnections ();
    void showBallonMessage ();
    void setSavedDataIcon (int id);
    void removeSavedDataIcon (int id);
    void renameConfig (int id, const QString &newName, const QString &oldName);
    void removeConfigInDatabase (int id);
    void addNewConfigToDatabase (const QString &name, const QString &path);
    QString internalBuild () const;

public slots:
    void setDisconnected (int id);
    void setError (int id, QString message);
    void userInputIsNeeded (int id, int type);
    void receivedIP (int id, QString ip);
    void receivedReconnect (int id);
    void receivedTapControl(int type);
    void receivedRemoveTap (QString state);
    void saveUserData (int id, int type, QString value, bool save);
    void receivedStatus (int id, bool isConnected, bool isConnecting, int lastAction, QString ip);

protected:
    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *event);

private:
    Preferences();
    static Preferences *mInst;

    // Enum für die Icons
    enum Icons {
        Inaktiv,
        Connected,
        Error,
        Connecting
    };

    QSystemTrayIcon *trayIcon;
    Ui::Preferences *m_ui;
    OpenVpn *actObject;

    // Methoden um feste Events erzeugen
    void createActions();
    // Methode um das Tray zu setzen
    void createTrayIcon();


    // Actions für Menu

    QAction *preferencesAction;
    QAction *quitAction;
    QAction *proxyAction;
    QAction *appInfoAction;

    QAction *mySubAction;
    QMenu *trayIconMenu;

    // Neues Netzwerk-Handling
    SslServer *server;
    //
    Database db;

    void refreshTapDeviceCount ();

    ParseXML *update;
    QString internalBuildValue;

    void centerMidle ();


private slots:
    void on_cmdImportConfig_clicked();
    void on_cmdRefreshData_clicked();
    void on_trvConnections_customContextMenuRequested(QPoint pos);
    void on_cmdNewConfig_clicked();
    void on_cmdToggleExtensions_clicked();
    void on_trvConnections_itemDoubleClicked(QTreeWidgetItem* item, int column);
    void on_cmdOpenInfo_clicked();
    void manageConnections ();
    void trayActivated(QSystemTrayIcon::ActivationReason reason);
    void openInfo ();
    void on_cmdClose_clicked();
    void closeApp ();

    void setStartConfig ();
    void removeStartConfig ();
    void removeErrorFromConfig ();

    void openProxySettings ();
    void openAppInfo ();
    void on_cmdToogleBallon_clicked();
    void on_cmdTooglePopup_clicked();
    void on_cmdToogleReconnect_clicked();
    void on_cmdAddNewTap_clicked();
    void on_cmdRemoveAllTap_clicked();
    void on_cmdToogleUseInteract_clicked();
    void on_cmdRemoveCredentials_clicked();
    void on_cmdShowCredentials_clicked();
    void on_cmdToogleSpalshScreen_clicked();
    void on_cmdToogleStartup_clicked();
    void on_cmdToogleWinEvent_clicked();
    void on_cmdCheckUpdate_clicked();
    void updateCheckIsReady (bool success, QString errMessage);
    void on_cmdUpdateSetting_clicked();
    void on_cmdOpenUpdate_clicked();
};

#endif // PREFERENCES_H
