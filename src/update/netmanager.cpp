#include "netmanager.h"
#include "ui_authenticationdialog.h"

#include <QtWidgets>

NetManager::NetManager(QObject *parent)
    : QNetworkAccessManager(parent)
{
    QObject::connect(this, SIGNAL(proxyAuthenticationRequired(QNetworkProxy,QAuthenticator*)), this, SLOT(proxyAuthenticationRequired(QNetworkProxy,QAuthenticator*)));
    //
    this->setProxySettings();
}

void NetManager::proxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *auth)
{
    static int count (0);

/*
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
*/

    //
    QDialog dlg;
    dlg.setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);
    dlg.adjustSize();
    //
    Ui::Dialog ui;
    ui.setupUi(&dlg);
    ui.passwordEdit->setEchoMode(QLineEdit::Password);
    ui.siteDescription->setText(QString("<qt>Connect to proxy \"%1\":</qt>").arg(proxy.hostName()));
    //
    if (dlg.exec() == QDialog::Accepted) {
        // Set the credentials
        auth->setUser(ui.userEdit->text());
        auth->setPassword(ui.passwordEdit->text());
    }
}

QNetworkReply *NetManager::createRequest(QNetworkAccessManager::Operation op, const QNetworkRequest &request, QIODevice *outgoingData)
{
    QNetworkRequest req = request;
    req.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, false);
    //
    QNetworkReply *reply = reply = QNetworkAccessManager::createRequest(op, req, outgoingData);
    //
    emit requestCreated(op, req, reply);
    //
    return reply;
}

void NetManager::setProxySettings()
{
    /*
    if (Settings::getInstance()->updateUseProxy()) {
        proxy.setHostName(Settings::getInstance()->updateProxyIp());
        proxy.setPort(Settings::getInstance()->updateProxyPort().toInt());
        proxy.setType(QNetworkProxy::HttpProxy);
        this->setProxy(proxy);
    }
    */
}
