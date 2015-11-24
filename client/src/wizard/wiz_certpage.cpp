#include "wiz_certpage.h"
#include "ui_wiz_certpage.h"

#include <qstandardpaths.h>

float windowsDpiScale();

CertPage::CertPage(QWidget *parent) :
    QWizardPage(parent),
    m_ui(new Ui::CertPage),
    lastDir ("")
{
    m_ui->setupUi(this);

    auto geom = this->geometry();
    auto size = geom.size() * windowsDpiScale();
    geom.setSize(size);
    this->setGeometry(geom);

    registerField(QLatin1String("txtCAPath*"), m_ui->txtCAPath);
    registerField(QLatin1String("txtCertPath*"), m_ui->txtCertPath);
    registerField(QLatin1String("txtKeyPath*"), m_ui->txtKeyPath);
    registerField(QLatin1String("txtServerCert"), m_ui->cbServerCert);

    this->setTitle("\n\n" + this->title());
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
    if (this->lastDir.isEmpty()) {
        QString homeDir;
        homeDir = QStandardPaths::locate(QStandardPaths::HomeLocation, homeDir, QStandardPaths::LocateOption::LocateDirectory);
        this->lastDir = homeDir;
    }

    QFileDialog caFileDialog (this);
    QString filename (caFileDialog.getOpenFileName(this, QObject::tr("Find root ca"), this->lastDir, QLatin1String("Certificates (*.crt *.cert *.pem);;All files (*.*)")));

    if (!filename.isEmpty()) {
        this->lastDir = filename.left(filename.lastIndexOf(QLatin1String("/")));
        m_ui->txtCAPath->setText(filename);
    }
}

void CertPage::on_cmdOpenCert_clicked()
{
    if (this->lastDir.isEmpty()) {
        QString homeDir;
        homeDir = QStandardPaths::locate(QStandardPaths::HomeLocation, homeDir, QStandardPaths::LocateOption::LocateDirectory);
        this->lastDir = homeDir;
    }

    QFileDialog certFileDialog;
    QString filename (certFileDialog.getOpenFileName(this, QObject::tr("Find certificates"), this->lastDir, QLatin1String("Certificates (*.crt *.cert *.pem);;All files (*.*)")));

    if (!filename.isEmpty()) {
        this->lastDir = filename.left(filename.lastIndexOf(QLatin1String("/")));
        m_ui->txtCertPath->setText(filename);
    }
}

void CertPage::on_cmdOpenKey_clicked()
{
    if (this->lastDir.isEmpty()) {
        QString homeDir;
        homeDir = QStandardPaths::locate(QStandardPaths::HomeLocation, homeDir, QStandardPaths::LocateOption::LocateDirectory);
        this->lastDir = homeDir;
    }

    QFileDialog keyFileDialog;
    QString filename (keyFileDialog.getOpenFileName(this, QObject::tr("Find key files"), this->lastDir, QLatin1String("Key files (*.key *.pem);;All files (*.*)")));

    if (!filename.isEmpty()) {
        this->lastDir = filename.left(filename.lastIndexOf(QLatin1String("/")));
        m_ui->txtKeyPath->setText(filename);
    }
}
