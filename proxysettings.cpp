#include "proxysettings.h"
#include "ui_proxysettings.h"

ProxySettings::ProxySettings(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::ProxySettings)
{
    m_ui->setupUi(this);

    m_ui->rbHttpProxy->setEnabled(false);
    m_ui->rbSocksProxy->setEnabled(false);
    m_ui->txtProxyIP->setEnabled(false);
    m_ui->txtProxyPort->setEnabled(false);

    QFile pINI (QApplication::applicationDirPath() + QString("/proxy.ini"));
    if (!pINI.exists()) {
        QMessageBox::critical(0,QString ("Securepoint VPN Client"), QString ("No proxy config available!"));
        return;
    }

    QSettings proxIni (QApplication::applicationDirPath() + QString("/proxy.ini"), QSettings::IniFormat);
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
    QFile pINI (QApplication::applicationDirPath() + QString("/proxy.ini"));
    if (!pINI.exists()) {
        QMessageBox::critical(0,QString ("Securepoint VPN Client"), QString ("No proxy config available!"));
        return;
    }
    QSettings proxIni (QApplication::applicationDirPath() + QString("/proxy.ini"), QSettings::IniFormat);
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
