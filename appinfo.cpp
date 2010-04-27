#include "appinfo.h"
#include "ui_appinfo.h"

#include "usercontroll.h"

#include <QDesktopServices>

AppInfo *AppInfo::mInst = NULL;

AppInfo *AppInfo::getInstance() {
    if (!mInst)
        mInst = new AppInfo ();
    return mInst;
}

AppInfo::AppInfo() :
    QDialog(),
    m_ui(new Ui::appInfo)
{
    // Window als Toolfenster ohne MinMax setzen
    this->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);
    m_ui->setupUi(this);
}

AppInfo::~AppInfo()
{
    delete m_ui;
}

void AppInfo::changeEvent(QEvent *e)
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

void AppInfo::on_cmdClose_clicked()
{
    this->close();
}


