#ifndef MANAGECONNECTION_H
#define MANAGECONNECTION_H

#include <QtCore>
#include <QShowEvent>
#include <QDialog>

#include "openvpn.h"
#include "appfunc.h"

namespace Ui {
    class ManageConnection;
}

class ManageConnection : public QDialog {
    Q_OBJECT
public:
    static ManageConnection *getInstance ();
    void setOpenVpnObject (OpenVpn *obj);

protected:
    void changeEvent(QEvent *e);
    void showEvent(QShowEvent *e);

private:
    Ui::ManageConnection *ui;
    static ManageConnection *mInst;
    OpenVpn *configObj;
    QString lastDir;

    // Methoden

    ManageConnection();
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
};

#endif // MANAGECONNECTION_H
