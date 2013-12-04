#include "appinfo.h"
#include "ui_appinfo.h"
#include <QDesktopServices>
#include "preferences.h"

AppInfo *AppInfo::mInst = NULL;

AppInfo *AppInfo::getInstance()
{
    if (!mInst)
        mInst = new AppInfo ();
    return mInst;
}

AppInfo::AppInfo() :
    QDialog(),
    m_ui(new Ui::appInfo)
{
    // Window als Toolfenster ohne MinMax setzen
    this->setWindowFlags(Qt::Tool);
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

void AppInfo::showEvent(QShowEvent *e)
{
    // Mittig ausrichten
    int winW = this->width();
    int winH = this->height();

    int left (0);
    int top (0);
    if (Preferences::instance()->isVisible()) {
        // Wenn das Hauptfenster offen ist mittig über diesem plazieren
        left = Preferences::instance()->geometry().x();
        top = Preferences::instance()->geometry().y();
        left = left + (Preferences::instance()->geometry().width() - winW) / 2;
        //
        top  = top + (Preferences::instance()->geometry().height() - winH) / 2;
    } else {
        // Desktop auswerten
        top = qApp->desktop()->height();
        left = qApp->desktop()->width();
        // Die Breite bei virtuellen Desktops vierteln
        if (left > 2000 && qApp->desktop()->isVirtualDesktop()) {
            left /= 4;
        } else {
            // Normaler Desktop
            left = (left - winH) / 2;
        }
        // Height
        if (top > 2000 && qApp->desktop()->isVirtualDesktop()) {
            top /= 4;
        } else {
            top = (top - winH) / 2;
        }
    }
    // Nun die neuen setzen
    this->setGeometry(left, top, winW, winH);
    // Öffnen
    e->accept();
    this->setWindowState(Qt::WindowActive);

    m_ui->lblVersionAndName->setText(QObject::tr("OpenVPN Client v1") + QLatin1String(" [") + Preferences::instance()->internalBuild() + QLatin1String("]"));
}


