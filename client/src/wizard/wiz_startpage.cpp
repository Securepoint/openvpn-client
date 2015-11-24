#include "wiz_startpage.h"
#include "ui_wiz_startpage.h"

#include <qabstractbutton.h>

float windowsDpiScale();

CStartPage::CStartPage(QWidget *parent) :
    QWizardPage(parent),
    m_ui(new Ui::CStartPage)
{
    m_ui->setupUi(this);

    auto geom = this->geometry();
    auto size = geom.size() * windowsDpiScale();
    geom.setSize(size);
    this->setGeometry(geom);

    this->setTitle("\n\n" + this->title());
    //geom.setTop(-scaleWhite);
    //m_ui->bannerLabel->setGeometry(geom);
    //m_ui->bannerLabel->setSize(geom.size());

    //setPixmap(QWizard::WatermarkPixmap, QPixmap(":/data/images/securepoint_wiz.png"));
    //this->setStyleSheet("background: none;");
}

CStartPage::~CStartPage()
{
    delete m_ui;
}

void CStartPage::changeEvent(QEvent *e)
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
