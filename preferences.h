#ifndef PREFERENCES_H
#define PREFERENCES_H

// Qt Includes
#include <QFileDialog>
#include <QSettings>
#include <QSystemTrayIcon>
#include <QtCore>
#include <QtGui>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

// Normale Includes
#include "appfunc.h"
#include "Configs.h"
#include "openvpn.h"
#include "openvpnqlistitem.h"
#include "settings.h"
#include "treebutton.h"
#include "treeconitem.h"

#include "tapdriver.h"

// Form Includes
#include "appinfo.h"
#include "frmgetuserdata.h"
#include "proxysettings.h"
#include "wiz_vpnwizard.h"
#include "deleteconfig.h"
#include "renameconfig.h"
#include "importconfig.h"
#include "manageconnection.h"


// Circle References vermeiden
class DataControll;
#include "datacontroll.h"

class ServiceLog;
#include "servicelog.h"


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

class Preferences : public QDialog {
    Q_OBJECT
public:
    Preferences(QWidget *parent = 0);
    ~Preferences();
    void openDialog (bool configFromCommandLine, QString commandLineConfig);
    void setVisible(bool visible);
    void refreshConfigList ();
    void setIcon (int index);
    void showTrayMessage(QString message, QSystemTrayIcon::MessageIcon messageType);    
    QStringList vpnLog;
    QSystemTrayIcon *trayIcon;    
    DataControll *data;
    QString configUser;
    QString configPwd;
    void startDaemon ();
    void searchStartConfigDir ();
    void refreshDialog ();
    void enableTreeButtons ();
    void setConnectionStatus ();
    bool isConnectionActive () const;


protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::Preferences *m_ui;
    OpenVpn *actObject;

    // Enum für die Icons
    enum Icons {
        Inaktiv,
        Connected,
        Error,
        Connecting
    };

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


private slots:    
    void on_cmdImportConfig_clicked();
    void on_cmdRefreshData_clicked();
    void on_trvConnections_customContextMenuRequested(QPoint pos);
    void on_cmdNewConfig_clicked();
    void on_cbForcePrivKey_toggled(bool checked);
    void on_cmdToggleExtensions_clicked();
    void on_trvConnections_itemDoubleClicked(QTreeWidgetItem* item, int column);    
    void on_cmdOpenInfo_clicked();    
    void manageConnections ();
    void trayActivated(QSystemTrayIcon::ActivationReason reason);
    void openInfo ();
    void on_cmdClose_clicked();
    void closeApp ();
    void startDelayDisconnectScript ();

    void showLogFromContextMenu ();

    void treePushButtonClicked ();

    void disableTreeButtons (TreeButton *button);
    void openProxySettings ();
    void openAppInfo ();
    void deleteLinkedConfig ();
    void deleteCryptedData ();
};

#endif // PREFERENCES_H
