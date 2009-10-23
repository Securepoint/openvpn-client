#include "wiz_startpage.h"
#include "ui_wiz_startpage.h"

StartPage::StartPage(QWidget *parent) :
    QWizardPage(parent),
    m_ui(new Ui::StartPage)
{
    m_ui->setupUi(this);
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/securepoint_wiz.png"));
}

StartPage::~StartPage()
{
    delete m_ui;
}

void StartPage::changeEvent(QEvent *e)
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
