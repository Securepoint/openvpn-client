#ifndef AdvConfigSettingsWidget_H
#define AdvConfigSettingsWidget_H

#include <QtCore>
#include <QShowEvent>
#include <QDialog>

#include <widgets/mainView/ConnectionData.h>

namespace Ui {
    class AdvConfigSettingsWidget;
}

#include <dialogs/framelessdialog.h>

class AdvConfigSettingsWidget : public FramelessDialog {
    Q_OBJECT
public:
    AdvConfigSettingsWidget(ConnectionData *obj, QWidget * parent = 0);

protected:
    void changeEvent(QEvent *e);
    void showEvent(QShowEvent *e);

private:
    Ui::AdvConfigSettingsWidget *ui;
    ConnectionData *configObj;
    QString lastDir;

    // Methoden
    void fillCipherCombo ();
    int getIndexFromCipher(QString cipher);
    void resetFields ();
    void fillFieldFromConfig ();
    QStringList getAllFieldWhichNotIntoTheInterface ();

    QString caValue;
    QString certValue;
    QString keyValue;
    QString pkcs12Value;

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
    void on_rbPkcs_toggled(bool checked);
    void on_cmdGetPkcs12Path_clicked();
    void on_rbNormal_toggled(bool checked);
    void on_cmdAdd_clicked();
    void on_cmdRemove_clicked();
};

#endif // AdvConfigSettingsWidget_H
