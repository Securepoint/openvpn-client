#ifndef PREFERENCES_H
#define PREFERENCES_H
#include "openvpn.h"
#include "Configs.h"
#include "openvpnqlistitem.h"
class VpnWizard;
#include "wiz_vpnwizard.h"
#include "appfunc.h"
#include "configexport.h"
class ImportConfig;
#include "importconfig.h"
#include "appfunc.h"
#include "appinfo.h"
class DeleteConfig;
#include "deleteconfig.h"

#include "tapdriver.h"
#include "proxysettings.h"

#include <QtGui/QDialog>
#include <QSystemTrayIcon>
#include <QFileDialog>


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
    void openDialog ();
    void setVisible(bool visible);
    void refreshConfigList ();
    void deleteConfigFromList (bool fconFile, bool fconCaFile, bool fconCertFile,
                               bool fconKeyFile, bool fconUserFile, bool fconScriptFile,
                               bool fconDir);
protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::Preferences *m_ui;
    void fillCipherCombo ();
    int getIndexFromCipher (QString cipher);
    void enableFields (bool flag);
    void resetFields ();
    OpenVpn *actObject;
    VpnWizard *vpnwiz;
    ConfigExport exportDialog;
    ImportConfig *importDialog;
    // Methoden um feste Events erzeugen
    void createActions();
    // Methode um das Tray zu setzen
    void createTrayIcon();
    void refreshDialog ();

    // Actions für Menu
    QAction *connectVpnAction;
    QAction *preferencesAction;
    QAction *infoAction;
    QAction *quitAction;
    QAction *importAction;
    QAction *proxyAction;
    //Preferences mydia;
    appInfo infoDialog;
    ProxySettings proxy;
    DeleteConfig *deleteDialog;

    QAction *mySubAction;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QMenu *trayTest;

    // Configlisten
    Configs myConfigs;
    QList<OpenVpn*> subMenuList;
    // Menuliste
    QList<QAction*> menuList;
    QList<QAction*> menuChildList;
    // Dir für Certs merken
    QString lastDir;
    QAction *conAct;

private slots:
    void on_cmdImportConfig_clicked();
    void manageConnections ();
    void trayActivated(QSystemTrayIcon::ActivationReason reason);
    void openInfo ();
    void importConfig ();
    void on_cmdConnect_clicked();
    void on_cmdErrorConnect_clicked();
    void on_cmdAfterDisconnect_clicked();
    void on_cmdBeforeDisconnect_clicked();
    void on_cmdAfterConnect_clicked();
    void on_cmdBeforeConnect_clicked();
    void on_cmdGetKeyPath_clicked();
    void on_cmdGetCertPath_clicked();
    void on_cmdGetCAPath_clicked();
    void on_cmdSave_clicked();
    void on_cmdAddConfig_clicked();
    void on_cmdClose_clicked();
    void openContextMenuListView (const QPoint &pos);
    void deleteConfig ();
    void editConfig ();
    void connectConfig ();
    void exportConfig ();
    void openConfigFromListView (QListWidgetItem * item );
    void closeApp ();
    void proxySettings ();
};

#endif // PREFERENCES_H
