#ifndef FRMGETUSERDATA_H
#define FRMGETUSERDATA_H

#include "framelessdialog.h"
#include "ui_frmgetuserdata.h"

// 0 - Username
// 1 - Pwd
// 2 - OTP
// 3 - PKCS12
// 4 - Private Key für Crypted User Data
namespace InputType {
    enum UserInputType {
        Username,
        Password,
        Otp,
        Pkcs12,
        PrivateKey,
        HttpUsername,
        HttpPassword,
        CryptPassword,
    };
}



namespace Ui {
    class FrmGetUserData;
}

class FrmGetUserData : public FramelessDialog {
    Q_OBJECT

public:    
    FrmGetUserData(InputType::UserInputType type, const QString &name, int id = -1, QWidget * parent = 0);
    ~FrmGetUserData();    
    void setFrmType ();

public slots:
    void receivedCloseMe ();

protected:
    void changeEvent(QEvent *e);
    void showEvent (QShowEvent *e);
    void closeEvent(QCloseEvent *e);

private:
    Ui::FrmGetUserData *ui;        
    bool dataAvail;
    InputType::UserInputType frmType;
    int vpnId;
    bool force;

private slots:
    void on_cmdOK_clicked();
    void on_cmdClose_clicked();
     void on_cmdClose_2_clicked();
signals:
    void writeUserData (QString data);
    void saveUserData (int id, int type, QString value, bool save);
    void cryptKey (QString key);

};

#endif // FRMGETUSERDATA_H

