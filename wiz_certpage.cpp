#include "wiz_certpage.h"
#include "ui_wiz_certpage.h"

CertPage::CertPage(QWidget *parent) :
    QWizardPage(parent),
    m_ui(new Ui::CertPage)
{
    m_ui->setupUi(this);
    registerField("txtCAPath*", m_ui->txtCAPath);
    registerField("txtCertPath*", m_ui->txtCertPath);
    registerField("txtKeyPath*", m_ui->txtKeyPath);
    registerField("txtServerCert", m_ui->cbServerCert);
}

CertPage::~CertPage()
{
    delete m_ui;
}

void CertPage::changeEvent(QEvent *e)
{
    QWizardPage::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void CertPage::on_cmdInfoServerCert_clicked()
{
    m_ui->memHelp->setText("Specify that a certificate is used, which is created with the server attribute.");
}

void CertPage::on_cmdInfoCA_clicked()
{
    m_ui->memHelp->setText("Specify the path to the root CA.\nThis field is required.");
}

void CertPage::on_cmdInfoCert_clicked()
{
    m_ui->memHelp->setText("Specify the path to the certificate.\nThis field is required.");
}

void CertPage::on_cmdInfoKey_clicked()
{
    m_ui->memHelp->setText("Specify the path to the key file.\nThis field is required.");
}

void CertPage::on_cmdOpenCA_clicked()
{
    QFileDialog caFileDialog;
    QString filename = caFileDialog.getOpenFileName(this, tr("Find root ca"), QApplication::applicationDirPath(), "Certificates (*.cert)");
    if (filename != "") {
        m_ui->txtCAPath->setText(filename);
    }
}

void CertPage::on_cmdOpenCert_clicked()
{
    QFileDialog certFileDialog;
    QString filename = certFileDialog.getOpenFileName(this, tr("Find certificates"), QApplication::applicationDirPath(), "Certificates (*.cert)");
    if (filename != "") {
        m_ui->txtCertPath->setText(filename);
    }
}

void CertPage::on_cmdOpenKey_clicked()
{
    QFileDialog keyFileDialog;
    QString filename = keyFileDialog.getOpenFileName(this, tr("Find key files"), QApplication::applicationDirPath(), "Key files (*.key)");
    if (filename != "") {
        m_ui->txtKeyPath->setText(filename);
    }
}
