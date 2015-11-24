#include "wizard.h"
#include "ui_wizard.h"


#include <qabstractbutton.h>

float windowsDpiScale();

Wizard::Wizard(QWidget *parent) :
    QWizard(parent),
    m_ui(new Ui::Wizard)
{
    m_ui->setupUi(this);

    //this->button(QWizard::WizardButton::BackButton)->setIcon(QIcon(":/data/images/back_small.png"));
    //this->button(QWizard::WizardButton::NextButton)->setIcon(QIcon(":/data/images/next_small.png"));
}

Wizard::~Wizard()
{
    delete m_ui;
}

void Wizard::changeEvent(QEvent *e)
{
    QWizard::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Wizard::on_Wizard_accepted()
{

}
