#include "wiz_advpage.h"
#include "ui_wiz_advpage.h"

AdvPage::AdvPage(QWidget *parent) :
    QWizardPage(parent),
    m_ui(new Ui::AdvPage)
{
    m_ui->setupUi(this);
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
    registerField(QLatin1String("txtUserPass"), m_ui->cbUserPass);

    // Windir
    // Fields
    registerField(QLatin1String("cbDefault"), m_ui->cbWinDirUseDefault);
    registerField(QLatin1String("cbOther"), m_ui->cbWinDirOther);
    registerField(QLatin1String("cbEnvironment"), m_ui->cbWinDirEnvironment);
    registerField(QLatin1String("cbPath"), m_ui->cbWinDirPath);

    registerField(QLatin1String("txtPath"), m_ui->txtPath);
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
    m_ui->cbUserPass->setChecked(true);

    m_ui->cmbRouteMethod->setCurrentIndex(1);

    m_ui->cbWinDirUseDefault->setChecked(true);
    m_ui->cbWinDirEnvironment->setChecked(true);
    m_ui->cbWinDirEnvironment->setEnabled(false);
    m_ui->cbWinDirPath->setEnabled(false);
    m_ui->txtPath->setEnabled(false);
    m_ui->txtPath->setText(QLatin1String(""));
}

void AdvPage::on_cbWinDirUseDefault_toggled(bool checked)
{

}

void AdvPage::on_cbWinDirOther_toggled(bool checked)
{
    m_ui->cbWinDirEnvironment->setEnabled(checked);
    m_ui->cbWinDirPath->setEnabled(checked);

    // Das Textfeld hat ein paar mehr Möglichkeiten
    if (!checked) {
        m_ui->txtPath->setEnabled(false);
    }

    if (checked && m_ui->cbWinDirPath->isChecked()) {
        m_ui->txtPath->setEnabled(true);
    }
}

void AdvPage::on_cbWinDirEnvironment_toggled(bool checked)
{

}

void AdvPage::on_cbWinDirPath_toggled(bool checked)
{
    m_ui->txtPath->setEnabled(checked);
}
