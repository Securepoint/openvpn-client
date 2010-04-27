#include "proxysettings.h"
#include "ui_proxysettings.h"

ProxySettings *ProxySettings::mInst = NULL;

ProxySettings *ProxySettings::getInstance() {
    if (!mInst)
        mInst = new ProxySettings ();
    return mInst;
}

ProxySettings::ProxySettings() :
    QDialog(),
    m_ui(new Ui::ProxySettings)
{
    m_ui->setupUi(this);
    this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint);
}

void ProxySettings::showEvent(QShowEvent *e) {
    m_ui->rbHttpProxy->setEnabled(false);
    m_ui->rbSocksProxy->setEnabled(false);
    m_ui->txtProxyIP->setEnabled(false);
    m_ui->txtProxyPort->setEnabled(false);

    QFile pINI (QApplication::applicationDirPath() + QString("/proxy.ini"));
    if (!pINI.exists()) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Securepoint SSL VPN"));
        msgBox.setText(tr("Proxy Settings"));
        msgBox.setWindowIcon(QIcon(":/images/logo.png"));
        msgBox.setInformativeText(tr("No proxy config available!"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
        msgBox.exec();        
        this->setEnabled(false);
    } else {
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
        // Mittig ausrichten
        int screenH = qApp->desktop()->height();
        int screenW = qApp->desktop()->width();
        int winH = 350;
        int winW = 300;
        // Nun die neuen setzen
        this->setGeometry((screenW / 2) - (winW / 2), (screenH / 2) - (winH / 2), winW, winH);
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
    QFile pINI (QApplication::applicationDirPath() + QString("/proxy.ini"));
    if (!pINI.exists()) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Securepoint SSL VPN"));
        msgBox.setText(tr("Proxy Settings"));
        msgBox.setWindowIcon(QIcon(":/images/logo.png"));
        msgBox.setInformativeText(tr("No proxy config available!"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
        msgBox.exec();
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
