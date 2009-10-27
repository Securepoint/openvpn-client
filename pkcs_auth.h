#ifndef PKCS_AUTH_H
#define PKCS_AUTH_H

#include <QtGui/QDialog>

#include <QProcess>

namespace Ui {
    class pkcs_auth;
}

class pkcs_auth : public QDialog {
    Q_OBJECT
public:
    pkcs_auth(QWidget *parent = 0);
    ~pkcs_auth();
    QProcess *parentProc;
    void clearFields ();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::pkcs_auth *m_ui;

private slots:
    void on_cmdOK_clicked();
};

#endif // PKCS_AUTH_H
