#ifndef STDUSERAUTH_H
#define STDUSERAUTH_H

#include <QtGui/QDialog>
class OpenVpn;
#include "openvpn.h"

namespace Ui {
    class StdUserAuth;
}

class StdUserAuth : public QDialog {
    Q_OBJECT
public:
    StdUserAuth(QWidget *parent = 0);
    ~StdUserAuth();
    QSystemTrayIcon *pIcon;
    OpenVpn *parentVpn;
    void clearFields ();
    void fillFromConfig ();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::StdUserAuth *m_ui;


private slots:
    void on_cmdOK_clicked();
    void on_cmdClose_clicked();
    void getAuth ();
};

#endif // STDUSERAUTH_H
