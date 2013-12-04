#include "wiz_remotepage.h"
#include "ui_wiz_remotepage.h"

RemotePage::RemotePage(QWidget *parent) :
    QWizardPage(parent),
    m_ui(new Ui::RemotePage)
{
    m_ui->setupUi(this);
    // fill Combo
    m_ui->cmbRemoteProtocol->insertItem(0, QLatin1String("TCP"));
    m_ui->cmbRemoteProtocol->insertItem(1, QLatin1String("UDP"));

    this->registerField(QLatin1String("txtRemoteIP*"), m_ui->txtRemoteIP);
    this->registerField(QLatin1String("txtRemotePort*"), m_ui->txtRemotePort);
    this->registerField(QLatin1String("txtRemoteProtocol"), m_ui->cmbRemoteProtocol);
}

RemotePage::~RemotePage()
{
    delete m_ui;
}

void RemotePage::changeEvent(QEvent *e)
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

void RemotePage::initializePage() {
    if (m_ui->txtRemotePort->text().isEmpty()) {
        m_ui->txtRemotePort->setText(QLatin1String("1194"));
    }
    m_ui->cmbRemoteProtocol->setCurrentIndex(1);
}
