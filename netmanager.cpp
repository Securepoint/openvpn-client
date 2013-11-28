#include "netmanager.h"
#include "ui_authenticationdialog.h"

#include "settings.h"

NetManager::NetManager(QObject *parent)
    : QNetworkAccessManager(parent)
{
    QObject::connect(this, SIGNAL(proxyAuthenticationRequired(QNetworkProxy,QAuthenticator*)), this, SLOT(proxyAuthenticationRequired(QNetworkProxy,QAuthenticator*)));
    this->setProxySettings();
}

void NetManager::proxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *auth)
{
    static int count (0);


    bool show(true);
    if (!Settings::getInstance()->updateProxyUser().isEmpty() && !Settings::getInstance()->updateProxyPassword().isEmpty()) {
        if (count == 5) {
            count = 0;
        } else {
            auth->setUser(Settings::getInstance()->updateProxyUser());
            auth->setPassword(Settings::getInstance()->updateProxyPassword());
            show = false;
            count++;
        }
    }

    // Autofill?
    if (!show) {
        return;
    }

    qApp->setOverrideCursor(Qt::ArrowCursor);
    QDialog dlg;
    Ui::Dialog ui;
    ui.setupUi(&dlg);
    dlg.adjustSize();
    ui.passwordEdit->setEchoMode(QLineEdit::Password);
    ui.siteDescription->setText(QString("<qt>Connect to proxy \"%1\":</qt>").arg(proxy.hostName()));
    dlg.setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);

    if (dlg.exec() == QDialog::Accepted) {
        auth->setUser(ui.userEdit->text());
        auth->setPassword(ui.passwordEdit->text());
        qApp->setOverrideCursor(Qt::WaitCursor);
    }
}

QNetworkReply *NetManager::createRequest(QNetworkAccessManager::Operation op, const QNetworkRequest &request, QIODevice *outgoingData)
{
    QNetworkReply *reply = 0;

    QNetworkRequest req = request;

    req.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, false);

    reply = QNetworkAccessManager::createRequest(op, req, outgoingData);
    //
    emit requestCreated(op, req, reply);
    return reply;
}

void NetManager::setProxySettings()
{
    if (Settings::getInstance()->updateUseProxy()) {
        proxy.setHostName(Settings::getInstance()->updateProxyIp());
        proxy.setPort(Settings::getInstance()->updateProxyPort().toInt());
        proxy.setType(QNetworkProxy::HttpProxy);
        this->setProxy(proxy);
    }
}
