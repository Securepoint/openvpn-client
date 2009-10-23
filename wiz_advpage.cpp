#include "wiz_advpage.h"
#include "ui_wiz_advpage.h"

AdvPage::AdvPage(QWidget *parent) :
    QWizardPage(parent),
    m_ui(new Ui::AdvPage)
{
    m_ui->setupUi(this);
    m_ui->cmbRouteMethod->insertItem(0, "Not defined");
    m_ui->cmbRouteMethod->insertItem(1, "Exe");
    m_ui->cmbRouteMethod->insertItem(2, "IPAPI");
    // register Fields
    registerField("txtMssFix", m_ui->cbMsfix);
    registerField("txtRouteMethod", m_ui->cmbRouteMethod);
    registerField("txtMute", m_ui->txtMute);
    registerField("txtVerbose", m_ui->txtVerbose);
    registerField("txtRouteDelay", m_ui->txtRouteDelay);
    registerField("txtCompLzo", m_ui->cbCompLzo);
    registerField("txtFloat", m_ui->cbFloat);
    registerField("txtNobind", m_ui->cbNobind);
    registerField("txtUserPass", m_ui->cbUserPass);
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
    if (m_ui->txtMute->text() == "")
        m_ui->txtMute->setText("20");
    if (m_ui->txtRouteDelay->text() == "")
        m_ui->txtRouteDelay->setText("2");
    if (m_ui->txtVerbose->text() == "")
        m_ui->txtVerbose->setText("3");
}

void AdvPage::on_cmdInfoAuthUserPass_clicked()
{
    m_ui->memHelp->setText("Specify that the user/pass authentication method is used.\nIt is required to type a username and a password for the connection.");
}

void AdvPage::on_cmdInfoWinRouteMethod_clicked()
{
    m_ui->memHelp->setText("Specify which method to use for adding routes on Windows?\nipapi - Use IP helper API.\nexe - Use the route.exe .");
}
