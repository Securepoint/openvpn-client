#include "wiz_endpage.h"
#include "ui_wiz_endpage.h"

#include "wiz_remotepage.h"

float windowsDpiScale();

CEndPage::CEndPage(QWidget *parent) :
    QWizardPage(parent),
    m_ui(new Ui::CEndPage)
{
    m_ui->setupUi(this);

    auto geom = this->geometry();
    auto size = geom.size() * windowsDpiScale();
    geom.setHeight(size.height());
    this->setGeometry(geom);
    size.setWidth(geom.width());
    this->setMinimumSize(size);

    this->setTitle("\n\n" + this->title());
}

CEndPage::~CEndPage()
{
    delete m_ui;
}

void CEndPage::changeEvent(QEvent *e)
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


void CEndPage::initializePage()
{
    m_ui->gbEndPage->setTitle(QString(QObject::tr("Data of new config:") + QLatin1String(" ")).append(field(QLatin1String("txtConfigName")).toString()));
    m_ui->lblRemoteIP->setText(field(QLatin1String("txtRemoteCount")).toString());
    if (field(QLatin1String("txtRemoteProtocol")).toInt() == 0) {
        m_ui->lblRemoteProtocol->setText(QLatin1String("TCP"));
    } else {
        m_ui->lblRemoteProtocol->setText(QLatin1String("UDP"));
    }
    m_ui->txtPathToCA->setText(field(QLatin1String("txtCAPath")).toString());
    m_ui->txtPathToCert->setText(field(QLatin1String("txtCertPath")).toString());
    m_ui->txtPathToKey->setText(field(QLatin1String("txtKeyPath")).toString());
    m_ui->lblCerttype->setText((field(QLatin1String("txtServerCert")).toBool() ? QLatin1String("Server") : QLatin1String("Normal")));
    m_ui->lblMssfix->setText((field(QLatin1String("txtMssFix")).toBool() ? QLatin1String("Yes") : QLatin1String("No")));
    QString sRouteMethod (field(QLatin1String("txtRouteMethod")).toInt() == 1 ? QLatin1String("Exe") : (field(QLatin1String("txtRouteMethod")).toInt() == 0 ? QLatin1String("Not defined") : QLatin1String("IAPI")));
    m_ui->lblRouteMethod->setText(sRouteMethod);
    m_ui->lblVerbose->setText(field(QLatin1String("txtVerbose")).toString());
    m_ui->lblMute->setText(field(QLatin1String("txtMute")).toString());
    m_ui->lblRouteDelay->setText(field(QLatin1String("txtRouteDelay")).toString());
    m_ui->lblCompLzo->setText((field(QLatin1String("txtCompLzo")).toBool() ? QLatin1String("Yes") : QLatin1String("No")));
    m_ui->lblFloat->setText((field(QLatin1String("txtFloat")).toBool() ? QLatin1String("Yes") : QLatin1String("No")));
    m_ui->lblNobind->setText((field(QLatin1String("txtNobind")).toBool() ? QLatin1String("Yes") : QLatin1String("No")));
    m_ui->lblUserPass->setText((field(QLatin1String("txtUserPass")).toBool() ? QLatin1String("Yes") : QLatin1String("No")));

    // Das Windir auswerten
    if (field(QLatin1String("cbDefault")).toBool()) {
        m_ui->lblWindir->setText(QObject::tr("default"));
    } else {
        // Custom path oder Env
        if (field(QLatin1String("cbEnvironment")).toBool()) {
            m_ui->lblWindir->setText(QObject::tr("Environment: ") + QLatin1String(getenv("windir")));
        } else {
            m_ui->lblWindir->setText(QObject::tr("Custom: ") + field(QLatin1String("txtPath")).toString());
        }
    }
}
