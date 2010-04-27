#ifndef FRMGETUSERDATA_H
#define FRMGETUSERDATA_H

#include <QDialog>
#include <QtCore>
#include <QShowEvent>
#include <QDesktopServices>
#include <QDesktopWidget>

namespace Ui {
    class FrmGetUserData;
}

class FrmGetUserData : public QDialog {
    Q_OBJECT
public:
    static FrmGetUserData *getInstance ();
    ~FrmGetUserData();
    bool isDataAvailable ();
    void setFrmType (int type);
    QString getDataField ();
    bool isSaveChecked ();
    void closeMe ();

protected:
    void changeEvent(QEvent *e);
    void showEvent (QShowEvent *e);

private:
    Ui::FrmGetUserData *ui;
    FrmGetUserData();
    static FrmGetUserData *mInst;
    bool dataAvail;
    int frmType;
    QString dataField;

private slots:
    void on_cmdOK_clicked();
    void on_cmdClose_clicked();

signals:
    void dataIsAvailable ();
};

#endif // FRMGETUSERDATA_H
