#ifndef MANAGECONNECTION_H
#define MANAGECONNECTION_H

#include <QtCore>
#include <QShowEvent>
#include <QDialog>

#include "openvpn.h"

namespace Ui {
    class ManageConnection;
}

class ManageConnection : public QDialog {
    Q_OBJECT
public:
    ManageConnection(OpenVpn *obj);

protected:
    void changeEvent(QEvent *e);
    void showEvent(QShowEvent *e);

private:
    Ui::ManageConnection *ui;    
    OpenVpn *configObj;
    QString lastDir;

    // Methoden
    void fillCipherCombo ();
    int getIndexFromCipher(QString cipher);
    void resetFields ();
    void fillFieldFromConfig ();
    QStringList getAllFieldWhichNotIntoTheInterface ();

private slots:
    void on_cmdErrorConnect_clicked();
    void on_cmdAfterDisconnect_clicked();
    void on_cmdBeforeDisconnect_clicked();
    void on_cmdAfterConnect_clicked();
    void on_cmdBeforeConnect_clicked();
    void on_cmdGetKeyPath_clicked();
    void on_cmdGetCertPath_clicked();
    void on_cmdGetCAPath_clicked();
    void on_cmdSave_clicked();
    void on_cmdClose_clicked();
    void on_cbWinDirOther_2_toggled(bool checked);
    void on_cbWinDirPath_2_toggled(bool checked);
    void on_rbPkcs_toggled(bool checked);
    void on_cmdGetPkcs12Path_clicked();
    void on_rbNormal_toggled(bool checked);
};

#endif // MANAGECONNECTION_H
