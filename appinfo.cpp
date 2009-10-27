#include "appinfo.h"
#include "ui_appinfo.h"

appInfo::appInfo(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::appInfo)
{
    m_ui->setupUi(this);
}

appInfo::~appInfo()
{
    delete m_ui;
}

void appInfo::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void appInfo::on_cmdClose_clicked()
{
    this->close();
}
