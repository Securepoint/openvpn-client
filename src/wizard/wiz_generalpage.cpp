#include "wiz_generalpage.h"
#include "ui_wiz_generalpage.h"

float windowsDpiScale();

GeneralPage::GeneralPage(QWidget *parent) :
    QWizardPage(parent),
    m_ui(new Ui::GeneralPage)
{
    m_ui->setupUi(this);

    auto geom = this->geometry();
    auto size = geom.size() * windowsDpiScale();
    geom.setSize(size);
    this->setGeometry(geom);

    registerField("txtConfigName", m_ui->txtConfigName);

    connect(m_ui->txtConfigName, SIGNAL(textChanged(const QString &)), this, SLOT(inputTextChanged()));

    this->setTitle("\n\n" + this->title());
}

GeneralPage::~GeneralPage()
{
    delete m_ui;
}

void GeneralPage::inputTextChanged()
{
    emit completeChanged();

    m_ui->txtConfigName->setFocus();
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

bool GeneralPage::isComplete() const
{
    if(m_ui->txtConfigName->text().isEmpty())
        return false;
    else
        return true;
}

void GeneralPage::initializePage()
{
    QWizardPage::initializePage();
}
