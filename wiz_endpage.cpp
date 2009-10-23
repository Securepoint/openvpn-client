#include "wiz_endpage.h"
#include "ui_wiz_endpage.h"

EndPage::EndPage(QWidget *parent) :
    QWizardPage(parent),
    m_ui(new Ui::EndPage)
{
    m_ui->setupUi(this);
}

EndPage::~EndPage()
{
    delete m_ui;
}

void EndPage::changeEvent(QEvent *e)
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

void EndPage::initializePage()
{
    m_ui->gbEndPage->setTitle(QString("Data of new config: ").append(field("txtConfigName").toString()));
    m_ui->lblRemoteIP->setText(field("txtRemoteIP").toString());
    m_ui->lblRemotePort->setText(field("txtRemotePort").toString());
    if (field("txtRemoteProtocol").toInt() == 0)
        m_ui->lblRemoteProtocol->setText("TCP");
    else
        m_ui->lblRemoteProtocol->setText("UDP");
    m_ui->txtPathToCA->setText(field("txtCAPath").toString());
    m_ui->txtPathToCert->setText(field("txtCertPath").toString());
    m_ui->txtPathToKey->setText(field("txtKeyPath").toString());
    m_ui->lblCerttype->setText((field("txtServerCert").toBool() ? "Server" : "Normal"));
    m_ui->lblMssfix->setText((field("txtMssFix").toBool() ? "Yes" : "No"));
    QString sRouteMethod;
    sRouteMethod = (field("txtRouteMethod").toInt() == 1 ? "Exe" : (field("txtRouteMethod").toInt() == 0 ? "Not defined" : "IAPI"));
    m_ui->lblRouteMethod->setText(sRouteMethod);
    m_ui->lblVerbose->setText(field("txtVerbose").toString());
    m_ui->lblMute->setText(field("txtMute").toString());
    m_ui->lblRouteDelay->setText(field("txtRouteDelay").toString());
    m_ui->lblCompLzo->setText((field("txtCompLzo").toBool() ? "Yes" : "No"));
    m_ui->lblFloat->setText((field("txtFloat").toBool() ? "Yes" : "No"));
    m_ui->lblNobind->setText((field("txtNobind").toBool() ? "Yes" : "No"));
    m_ui->lblUserPass->setText((field("txtUserPass").toBool() ? "Yes" : "No"));

}
