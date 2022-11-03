#ifndef FRMGETUSERDATA_H
#define FRMGETUSERDATA_H

#include "framelessdialog.h"
#include "ui_sslcertsdialog.h"

namespace Ui {
    class SSLDialog;
}

class SSLDialog : public FramelessDialog {
    Q_OBJECT

public:    
    SSLDialog(const QString &certificateInfo, QWidget * parent = 0);
    ~SSLDialog();
    bool isCertificateAccepted();

public slots:
    void receivedCloseMe ();

protected:
    void changeEvent(QEvent *e);
    void showEvent (QShowEvent *e);
    void closeEvent(QCloseEvent *e);

private:
    Ui::SSLDialog *ui;
    bool certificateAcceptState;

private slots:
    void on_CmdYes_clicked();
    void on_cmdNo_clicked();
};

#endif // FRMGETUSERDATA_H

