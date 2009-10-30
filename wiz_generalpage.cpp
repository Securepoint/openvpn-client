#include "wiz_generalpage.h"
#include "ui_wiz_generalpage.h"

GeneralPage::GeneralPage(QWidget *parent) :
    QWizardPage(parent),
    m_ui(new Ui::GeneralPage)
{
    m_ui->setupUi(this);
    registerField("txtConfigName*", m_ui->txtConfigName);
}

GeneralPage::~GeneralPage()
{
    delete m_ui;
}

void GeneralPage::changeEvent(QEvent *e)
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

void GeneralPage::initializePage()
{
    m_ui->txtConfigName->setFocus();
}
