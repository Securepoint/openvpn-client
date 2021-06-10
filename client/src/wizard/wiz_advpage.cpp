#include "wiz_advpage.h"
#include "ui_wiz_advpage.h"

float windowsDpiScale();

AdvPage::AdvPage(QWidget *parent) :
    QWizardPage(parent),
    m_ui(new Ui::AdvPage)
{
    m_ui->setupUi(this);

    auto geom = this->geometry();
    auto size = geom.size() * windowsDpiScale();
    geom.setSize(size);
    this->setGeometry(geom);

    m_ui->cmbRouteMethod->insertItem(0, QLatin1String("Not defined"));
    m_ui->cmbRouteMethod->insertItem(1, QLatin1String("EXE"));
    m_ui->cmbRouteMethod->insertItem(2, QLatin1String("IPAPI"));
    // register Fields
    registerField(QLatin1String("txtMssFix"), m_ui->cbMsfix);
    registerField(QLatin1String("txtRouteMethod"), m_ui->cmbRouteMethod);
    registerField(QLatin1String("txtMute"), m_ui->txtMute);
    registerField(QLatin1String("txtVerbose"), m_ui->txtVerbose);
    registerField(QLatin1String("txtRouteDelay"), m_ui->txtRouteDelay);
    registerField(QLatin1String("txtCompLzo"), m_ui->cbCompLzo);
    registerField(QLatin1String("txtFloat"), m_ui->cbFloat);
    registerField(QLatin1String("txtNobind"), m_ui->cbNobind);
    registerField(QLatin1String("txtUserPass"), m_ui->cbAuthUserPass);

    this->setTitle("\n\n" + this->title());
}

AdvPage::~AdvPage()
{
    delete m_ui;
}

void AdvPage::changeEvent(QEvent *e)
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

void AdvPage::initializePage() {
    if (m_ui->txtMute->text().isEmpty()) {
        m_ui->txtMute->setText(QLatin1String("20"));
    }
    if (m_ui->txtRouteDelay->text().isEmpty()) {
        m_ui->txtRouteDelay->setText(QLatin1String("2"));
    }

    if (m_ui->txtVerbose->text().isEmpty()) {
        m_ui->txtVerbose->setText(QLatin1String("3"));
    }

    m_ui->cbCompLzo->setChecked(false);
    m_ui->cbFloat->setChecked(true);
    m_ui->cbMsfix->setChecked(true);
    m_ui->cbNobind->setChecked(true);
    m_ui->cbAuthUserPass->setChecked(true);

    m_ui->cmbRouteMethod->setCurrentIndex(1);

}
