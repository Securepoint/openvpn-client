#ifndef USERAUTH_H
#define USERAUTH_H

#include <QObject>
#include <QDialog>
class OpenVpn;
#include "openvpn.h"

QT_BEGIN_NAMESPACE
    class QLabel;
    class QLineEdit;
    class QTextEdit;
    class QPushButton;
    class QGroupBox;
QT_END_NAMESPACE



class UserAuth : public QDialog
{
    Q_OBJECT

public:
    UserAuth();
    QSystemTrayIcon *pIcon;
    OpenVpn *parentVpn;
    void clearFields ();

private:
    QLabel *labelUser;
    QLabel *labelPwd;
    QPushButton *cmdOK;
    QPushButton *cmdCancel;
    QLineEdit *txtUser;
    QLineEdit *txtPwd;
    QGroupBox *authGroupBox;

private slots:
    void getAuth ();
    void closeDialog ();
};

#endif // USERAUTH_H
