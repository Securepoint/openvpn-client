#ifndef PREFERENCES_H
#define PREFERENCES_H
#include "openvpn.h"
#include "openvpnqlistitem.h"
#include "wiz_vpnwizard.h"
#include "appfunc.h"
#include "configexport.h"
#include "importconfig.h"

#include <QtGui/QDialog>
#include <QFileDialog>


namespace Ui {
    class Preferences;
}

class Preferences : public QDialog {
    Q_OBJECT
public:
    Preferences(QWidget *parent = 0);
    ~Preferences();
    void openDialog (QList<OpenVpn*> configList);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::Preferences *m_ui;
    void fillCipherCombo ();
    int getIndexFromCipher (QString cipher);
    void enableFields (bool flag);
    void resetFields ();
    OpenVpn *actObject;
    VpnWizard vpnwiz;
    ConfigExport exportDialog;
    ImportConfig importDialog;

private slots:
    void on_cmdImport_clicked();
    void on_cmdConnect_clicked();
    void on_cmdInfoScriptsDelyAC_clicked();
    void on_cmdErrorConnect_clicked();
    void on_cmdAfterDisconnect_clicked();
    void on_cmdBeforeDisconnect_clicked();
    void on_cmdAfterConnect_clicked();
    void on_cmdBeforeConnect_clicked();
    void on_cmdInfoScriptsErrorConnect_clicked();
    void on_cmdInfoScriptsAfterDisconnect_clicked();
    void on_cmdInfoScriptsBeforeDisconnect_clicked();
    void on_cmdInfoScriptsBeforeConnect_clicked();
    void on_cmdInfoScriptsAfterConnect_clicked();
    void on_cmdInfoAuthUserPass_clicked();
    void on_cmdInfoDevName_clicked();
    void on_cmdInfoServerCertificate_clicked();
    void on_cmdInfoMuteWirelessWarnings_clicked();
    void on_cmdInfoAdvRandomHost_clicked();
    void on_cmdInfoWinDevNode_clicked();
    void on_cmdInfoWinRouteMethod_clicked();
    void on_cmdInfoLinuxUser_clicked();
    void on_cmdInfoRemoteIP_clicked();
    void on_cmdInfoRemote_clicked();
    void on_cmdInfoClient_clicked();
    void on_label_linkHovered(QString link);
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
};

#endif // PREFERENCES_H
