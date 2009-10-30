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
    this->lastDir = QString ("");
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

void CertPage::on_cmdOpenCA_clicked()
{
    if (this->lastDir == "") {
        this->lastDir = QApplication::applicationDirPath();
    }
    QFileDialog caFileDialog;
    QString filename = caFileDialog.getOpenFileName(this, tr("Find root ca"), this->lastDir, "Certificates (*.cert *.pem)");
    if (filename != "") {
        this->lastDir = filename.left(filename.lastIndexOf("/"));
        m_ui->txtCAPath->setText(filename);
    }
}

void CertPage::on_cmdOpenCert_clicked()
{
    if (this->lastDir == "") {
        this->lastDir = QApplication::applicationDirPath();
    }
    QFileDialog certFileDialog;
    QString filename = certFileDialog.getOpenFileName(this, tr("Find certificates"), this->lastDir, "Certificates (*.cert *.pem)");
    if (filename != "") {
        this->lastDir = filename.left(filename.lastIndexOf("/"));
        m_ui->txtCertPath->setText(filename);
    }
}

void CertPage::on_cmdOpenKey_clicked()
{
    if (this->lastDir == "") {
        this->lastDir = QApplication::applicationDirPath();
    }
    QFileDialog keyFileDialog;
    QString filename = keyFileDialog.getOpenFileName(this, tr("Find key files"), this->lastDir, "Key files (*.key *.pem)");
    if (filename != "") {
        this->lastDir = filename.left(filename.lastIndexOf("/"));
        m_ui->txtKeyPath->setText(filename);
    }
}
