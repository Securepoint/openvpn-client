#include "proxysettings.h"
#include "ui_proxysettings.h"

#include "message.h"
#include "preferences.h"
#include "settings.h"

ProxySettings *ProxySettings::mInst = NULL;

ProxySettings *ProxySettings::getInstance()
{
    if (!mInst)
        mInst = new ProxySettings ();
    return mInst;
}

ProxySettings::ProxySettings() :
    QDialog(),
    m_ui(new Ui::ProxySettings)
{
    m_ui->setupUi(this);
    this->setWindowFlags(Qt::WindowCloseButtonHint);
}

void ProxySettings::showEvent(QShowEvent *e)
{

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

    m_ui->rbHttpProxy->setEnabled(false);
    m_ui->rbSocksProxy->setEnabled(false);
    m_ui->txtProxyIP->setEnabled(false);
    m_ui->txtProxyPort->setEnabled(false);

    QFile pINI (Settings::getInstance()->getProxyIniPath());
    if (!pINI.exists()) {
        //Message::error(QObject::tr("No proxy config available!"), QObject::tr("Proxy Settings"));
        m_ui->rbUseConfig->setChecked(true);
        m_ui->rbUseIE->setEnabled(true);
        m_ui->rbUseManual->setEnabled(true);
        m_ui->rbHttpProxy->setEnabled(false);
        m_ui->rbSocksProxy->setEnabled(false);
        m_ui->txtProxyIP->setEnabled(false);
        m_ui->txtProxyPort->setEnabled(false);
    } else {
        QSettings proxIni (Settings::getInstance()->getProxyIniPath(), QSettings::IniFormat);
        if (proxIni.value("proxy-use","").toString() == "CONFIG") {
            m_ui->rbUseConfig->setChecked(true);
        } else if (proxIni.value("proxy-use","").toString() == "IE") {
            m_ui->rbUseIE->setChecked(true);
        } else {
            // MANUAL
            m_ui->rbHttpProxy->setEnabled(true);
            m_ui->rbSocksProxy->setEnabled(true);
            m_ui->txtProxyIP->setEnabled(true);
            m_ui->txtProxyPort->setEnabled(true);
            m_ui->rbUseManual->setChecked(true);
            m_ui->txtProxyPort->setText(proxIni.value("proxy-port","8080").toString());
            m_ui->txtProxyIP->setText(proxIni.value("proxy-ip","").toString());
            if (proxIni.value("proxy-type","").toString() == "HTTP")
                m_ui->rbHttpProxy->setChecked(true);
            else
                m_ui->rbSocksProxy->setChecked(true);

        }
        #ifdef Q_OS_WIN32
            m_ui->rbUseIE->setEnabled(true);
        #endif

        // Öffnen
        e->accept();
        this->setWindowState(Qt::WindowActive);
    }
}

ProxySettings::~ProxySettings()
{
    delete m_ui;
}

void ProxySettings::changeEvent(QEvent *e)
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

void ProxySettings::on_cmdClose_clicked()
{
    this->close();
}

void ProxySettings::on_cmdSave_clicked()
{    
    QSettings proxIni (Settings::getInstance()->getProxyIniPath(), QSettings::IniFormat);
    if (m_ui->rbUseConfig->isChecked()) {
        proxIni.setValue("proxy-use", "CONFIG");
    } else if (m_ui->rbUseIE->isChecked()) {
        proxIni.setValue("proxy-use", "IE");
    } else {
        proxIni.setValue("proxy-use", "MANUAL");
        proxIni.setValue("proxy-type", (m_ui->rbHttpProxy->isChecked() ? "HTTP" : "SOCKS"));
        proxIni.setValue("proxy-ip", m_ui->txtProxyIP->text());
        proxIni.setValue("proxy-port", m_ui->txtProxyPort->text());
    }
    this->close();
}

void ProxySettings::on_rbUseManual_toggled(bool checked)
{
    if (checked) {
        m_ui->rbHttpProxy->setEnabled(true);
        m_ui->rbSocksProxy->setEnabled(true);
        m_ui->txtProxyIP->setEnabled(true);
        m_ui->txtProxyPort->setEnabled(true);
    } else {
        m_ui->rbHttpProxy->setEnabled(false);
        m_ui->rbSocksProxy->setEnabled(false);
        m_ui->txtProxyIP->setEnabled(false);
        m_ui->txtProxyPort->setEnabled(false);
    }
}
