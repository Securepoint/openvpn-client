#include "pkcs_auth.h"
#include "ui_pkcs_auth.h"

pkcs_auth::pkcs_auth(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::pkcs_auth)
{
    m_ui->setupUi(this);
}

pkcs_auth::~pkcs_auth()
{
    delete m_ui;
}

void pkcs_auth::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void pkcs_auth::clearFields() {
    m_ui->txtPKCSPwd->setText("");
}

void pkcs_auth::on_cmdOK_clicked()
{
    QByteArray ba;
    ba.append(m_ui->txtPKCSPwd->text().trimmed() + "\n");
    this->parentProc->write(ba);
    this->close();
}
