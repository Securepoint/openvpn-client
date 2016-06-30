#include "netmanager.h"
#include "ui_authenticationdialog.h"

#include <QtWidgets>

NetManager::NetManager(QObject *parent)
    : QNetworkAccessManager(parent)
{
    QObject::connect(this, SIGNAL(proxyAuthenticationRequired(QNetworkProxy,QAuthenticator*)), this, SLOT(proxyAuthenticationRequired(QNetworkProxy,QAuthenticator*)));
}

void NetManager::proxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *auth)
{
    static int count (0);
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
    QNetworkReply *reply = QNetworkAccessManager::createRequest(op, req, outgoingData);
    //
    emit requestCreated(op, req, reply);
    //
    return reply;
}
