#include "wizardpage.h"
#include "ui_wizardpage.h"

WizardPage::WizardPage(QWidget *parent) :
    QWizardPage(parent),
    m_ui(new Ui::WizardPage)
{
    m_ui->setupUi(this);
    setPixmap(QWizard::BannerPixmap, QPixmap(":/images/banner.png"));
}

WizardPage::~WizardPage()
{
    delete m_ui;
}

void WizardPage::changeEvent(QEvent *e)
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
