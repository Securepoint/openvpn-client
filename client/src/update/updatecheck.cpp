#include "updatecheck.h"

#include <QMessageBox>
#include <QtCore>

#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkProxy>
#include <QtNetwork/QAuthenticator>
#include <QMessageBox>
#include <ui_sslcertsdialog.h>
#include <ui_authenticationdialog.h>
#include <utils.h>
#include <crypt.h>
#include <frmmain.h>
#include <message.h>
#include <proxysettings.h>
#include <sslcertsdialog.h>

UpdateCheck::UpdateCheck()
    : currentMajor(2),
      currentMinor(0),
      currentPatch(38)
{
    // Create a new manager for this action
    manager = new QNetworkAccessManager();
    //
    QObject::connect(manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), this, SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
    QObject::connect(manager, SIGNAL(proxyAuthenticationRequired(const QNetworkProxy&, QAuthenticator*)), this, SLOT(proxyAuthenticationRequired(const QNetworkProxy&, QAuthenticator*)));
    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));

    // Init proxy stuff, if needed
    this->loadProxySettings();
}

UpdateCheck::~UpdateCheck()
{
    delete this->manager;
}


void UpdateCheck::finished(QNetworkReply *reply)
{
    //
    reply->deleteLater();

    // no error in request
    if (reply->error() != QNetworkReply::NoError){
        qDebug() << "reply error" << reply->errorString();
        return;
    }

    // get HTTP status code
    qint32 httpStatusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    //
    if (httpStatusCode < 200 || httpStatusCode > 300) {
        qDebug() << "status code " << httpStatusCode;
        return;
    }

    // All fine
    QString response (reply->readAll());
    //
    response.remove("\n");
    response = response.trimmed();
    // String needs 3 dot delimitered values major.minor.patch
    if (response.isEmpty()) {
        //
        qDebug() << "invalid respone(empty)";
        //
        return;
    }

    QStringList valueList (response.split("."));
    // Check if its the right format
    if (valueList.size() != 3) {
        //
        qDebug() << "invalid value list size";
        //
        return;
    }

    bool majorOkay;
    int major (valueList.value(0).toInt(&majorOkay));
    //
    if (!majorOkay) {
        //
        qDebug() << "invalid major";
        //
        return;
    }

    bool minorOkay;
    int minor (valueList.value(1).toInt(&minorOkay));
    //
    if (!minorOkay) {
        //
        qDebug() << "invalid minor";
        //
        return;
    }

    bool patchOkay;
    int patch (valueList.value(2).toInt(&patchOkay));
    //
    if (!patchOkay) {
        //
        qDebug() << "invalid patch";
        //
        return;
    }

    // Is a newer version available?
    bool newerVersionAvailable(false);
    //
    if (major > this->currentMajor) {
        newerVersionAvailable = true;
    }

    //
    if (minor > this->currentMinor) {
        newerVersionAvailable = true;
    }

    //
    if (patch > this->currentPatch) {
        newerVersionAvailable = true;
    }

    //
    if (newerVersionAvailable) {
        //
        emit updateAvailable((QString::number(major) + "." + QString::number(minor) + "." + QString::number(patch)));
    }
}

QString UpdateCheck::formatCertificate(QSslCertificate sslCertificate)
{
    QStringList certificateInformations;

    certificateInformations << QObject::tr("<b>Common name:</b> %1").arg(sslCertificate.subjectInfo(QSslCertificate::CommonName).join(""));
    certificateInformations << QObject::tr("<br /><b>Certificate issuer:</b> %1").arg(sslCertificate.issuerInfo(QSslCertificate::CommonName).join(""));
    certificateInformations << QObject::tr("<br /><b>Effective date:</b> %1").arg(sslCertificate.effectiveDate().toString());
    certificateInformations << QObject::tr("<br /><b>Expire date:</b> %1").arg(sslCertificate.expiryDate().toString());

    QMultiMap<QSsl::AlternativeNameEntryType, QString> subjectAlternativeNames = sslCertificate.subjectAlternativeNames();

    if (subjectAlternativeNames.count() > 0) {
        //
        QStringList alternativeNamesData;
        //
        alternativeNamesData << QLatin1String("<br /><br />");
        alternativeNamesData << QObject::tr("Alternate Names:");
        alternativeNamesData << QLatin1String("<ul><li>");
        alternativeNamesData << QStringList(subjectAlternativeNames.values(QSsl::DnsEntry)).join(QLatin1String("</li><li>"));
        alternativeNamesData << QLatin1String("</li></ul>");
        //
        certificateInformations << alternativeNamesData.join("");
    }

    certificateInformations << QLatin1String ("<br />================<br />");

    return QString("<p>%1</p>").arg(certificateInformations.join(QLatin1String("<br />")));
}
void UpdateCheck::onError(QNetworkReply::NetworkError code)
{
    qDebug() << "onError: " << code;
}

QByteArray UpdateCheck::acceptedCertificates()
{
    //
    QSettings settings (Utils::userApplicationDataDirectory() + QLatin1String ("/certificates.ini"), QSettings::IniFormat);
    QString availableCertificates (settings.value(QLatin1String("certs")).toString());
    // Decode
    availableCertificates = Crypt::decodeToPlaintext(availableCertificates);
    //
    return availableCertificates.toLatin1();
}

void UpdateCheck::setAcceptedCertificates(const QByteArray &data)
{
    //
    QSettings settings (Utils::userApplicationDataDirectory() + QLatin1String ("/certificates.ini"), QSettings::IniFormat);
    settings.setValue(QLatin1String("certs"), Crypt::encodePlaintextAscii(data));
}

void UpdateCheck::loadProxySettings()
{
    if (!ProxySettings::instance()->settingsAvailable() || ProxySettings::instance()->proxyTypeInUse() == "CONFIG") {
        return;
    }

    // IE OR MANUAL
    if (ProxySettings::instance()->proxyTypeInUse() == "IE") {
        this->proxy.setType(QNetworkProxy::HttpProxy);
        this->proxy.setHostName(ProxySettings::instance()->systemProxyHost());
        this->proxy.setPort(ProxySettings::instance()->systemProxyPort().toInt());
    } else {
        // Manual
        this->proxy.setType((ProxySettings::instance()->isHttpProxy() ? QNetworkProxy::HttpProxy : QNetworkProxy::Socks5Proxy));
        this->proxy.setHostName(ProxySettings::instance()->proxyHost());
        this->proxy.setPort(ProxySettings::instance()->proxyPort().toInt());
    }

    this->manager->setProxy(this->proxy);
}

void UpdateCheck::sslErrors(QNetworkReply *reply, const QList<QSslError> &sslErrors)
{
    // Load the accepted CAs
    QList<QSslCertificate> acceptedCAs = QSslCertificate::fromData(this->acceptedCertificates());
    // Now handle the error
    QStringList sslErrorStrings;
    QList<QSslCertificate> possibleNewCAs;
    // Loop through all errors and build a single string
    for (int sslErrorIndex = 0; sslErrorIndex < sslErrors.count(); ++sslErrorIndex) {
        // Get the current error
        const QSslError sslError (sslErrors.value(sslErrorIndex));
        // Check if we already handled the certificate which caused the error
        // and if the user accepted it
        if (acceptedCAs.contains(sslError.certificate())) {
            // Yes, nothing more to do
            continue;
        }

        // The self signed etc. certificate is not in our trusted ca list
        // so we need to handle this error
        sslErrorStrings << sslError.errorString();

        // Add the certificate which caused this error to the possible new ca list
        if (!sslError.certificate().isNull()) {
            possibleNewCAs.append(sslError.certificate());
        }
    }

    // Seems no error needed to be handled
    if (sslErrorStrings.isEmpty()) {
        //
        reply->ignoreSslErrors(sslErrors);
        //
        return;
    }

    // We need to handle the ssl error
    bool ignoreError(Message::confirm(QObject::tr("While loading <br /><br /> <b>%1 </b> <br /> <br /> an error occurred. <br /><br /><b> %2 </b><br /><br />Do you wish to ignore this error?")
                                   .arg(reply->url().toString())
                                   .arg(sslErrorStrings.join("")), QObject::tr("SSL Error")));
    if(ignoreError) {
        // Okay, now we can check for a certificate error
        // Ask the user if he want to connect anyway. If yes,
        // we'll save the ca in a restricted area.
        if (possibleNewCAs.count() > 0) {
            QStringList certificatesInformations;
            for (int newPossibleCAIndex = 0; newPossibleCAIndex < possibleNewCAs.count(); ++newPossibleCAIndex) {
                certificatesInformations << this->formatCertificate (possibleNewCAs.value(newPossibleCAIndex));
            }

            SSLDialog dialog(certificatesInformations.join(QString()));
            qDebug() << "before exec";
            dialog.exec();
            qDebug() << "after exec";
            // Do the user accepted the certificate?
            // If yes write it to our accepted list
            if (dialog.isCertificateAccepted()) {
                // Add the new certificates to our accepted list
                acceptedCAs.append(possibleNewCAs);

                // Get the current configuration
                QSslConfiguration currentSslConfiguration = QSslConfiguration::defaultConfiguration();
                QList<QSslCertificate> currentlyAllowedCAs = currentSslConfiguration.caCertificates();
                // Append the new CAs
                currentlyAllowedCAs.append(possibleNewCAs);
                // Set the new list
                currentSslConfiguration.setCaCertificates(currentlyAllowedCAs);
                // Assign it to our default configuration
                QSslConfiguration::setDefaultConfiguration(currentSslConfiguration);
                // Force that we needed this configuration with this reply
                reply->setSslConfiguration(currentSslConfiguration);

                // Save the new CAs to disk in pem format
                QByteArray certificatesAsPem;
                for (int acceptedCAIndex = 0; acceptedCAIndex < acceptedCAs.count(); ++acceptedCAIndex) {
                    certificatesAsPem += acceptedCAs.value(acceptedCAIndex).toPem() + '\n';
                }

                // Save it
                this->setAcceptedCertificates(certificatesAsPem);

                // Proceed loading the data
                reply->ignoreSslErrors(sslErrors);
            }
        }
    }
}

void UpdateCheck::proxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *authenticator)
{
    // We store the user credeantials due to a better user experience
    static QString cacheUserName ("");
    static QString cacheUserPassword ("");

    // Saving the password isn't a good idea, because if its wrong the client use it anyway.
    // This sometimes result in an account suspension within microsoft active directory.
    // Most ADs have a failure count for the login of 5, afterwards the account is suspended.

    // To prevent this, we ask the user again if the time difference of the password request is
    // lower than 2 seconds. Then it seems, the stored credentials are wrong and we ask again.
    // If the time difference is greater than 2 seconds it seems, that the crendials are correct
    // and we can use them for the sake of convienance.
    static QDateTime lastAuthenticationRequest = QDateTime::currentDateTime();

    // Check time diff
    if (lastAuthenticationRequest.msecsTo(QDateTime::currentDateTime()) < 2000) {
        // Okay, seems we got invalid credentials, ask again
        cacheUserName = "";
        cacheUserPassword = "";
    }

    // Check if we autofill the credentials or if we should ask
    if (cacheUserName.isEmpty() || cacheUserPassword.isEmpty()) {
        // Ask the user for the proxy credentials
        QDialog credentialsDialog;
        Ui::Dialog credentialsUi;
        // Assign the ui to the raw dialog
        credentialsUi.setupUi(&credentialsDialog);
        // Resize the dialog
        credentialsDialog.adjustSize();
        // Init dialog
        credentialsUi.passwordEdit->setEchoMode(QLineEdit::Password);
        credentialsUi.siteDescription->setText(QObject::tr("<qt>Connect to proxy \"%1\":</qt>").arg(QString(proxy.hostName()).toHtmlEscaped()));

        // Set the style
        credentialsUi.buttonBox->setStyleSheet(QLatin1String("color: #ffffff;"));
        credentialsUi.siteDescription->setStyleSheet(QLatin1String("color: #ffffff;"));
        credentialsUi.label->setStyleSheet(QLatin1String("color: #ffffff;"));
        credentialsUi.label_2->setStyleSheet(QLatin1String("color: #ffffff;"));
        credentialsUi.label_3->setStyleSheet(QLatin1String("color: #ffffff;"));
        credentialsUi.label_4->setStyleSheet(QLatin1String("color: #ffffff;"));

        // Do the user clicked okay
        if (credentialsDialog.exec() != QDialog::Accepted) {
            // No
            return;
        }

        // Yes, update the cache, we set the authenticator values later
        cacheUserName = credentialsUi.userEdit->text();
        cacheUserPassword = credentialsUi.passwordEdit->text();

        // Update the global values for the external browser, this prevents that the external browser will also ask for the proxy credentials
        //GlobalValues::proxyUsername = cacheUserName;
        //GlobalValues::proxyPassword = cacheUserPassword;
    }

    // Set new last authentication request
    lastAuthenticationRequest = QDateTime::currentDateTime();

    // Set the authenticator values
    authenticator->setUser(cacheUserName);
    authenticator->setPassword(cacheUserPassword);
}


void UpdateCheck::run()
{
    // Check if disable run file is available
    QFile disableRunFile (qApp->applicationDirPath() + QLatin1String("/update.disable.run.override"));
    if (disableRunFile.exists()) {
        qDebug() << "update check is disabled";
        //
        return;
    }

    QSslConfiguration sslConfiguration(QSslConfiguration::defaultConfiguration());
    // Later we use QSsl::TlsV1_3OrLater, needed min Qt 5.12
    sslConfiguration.setProtocol(QSsl::TlsV1_2OrLater);

    QNetworkRequest request;
    // Follow the redirects, needed min Qt 5.6
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    // We force only allow TLS 1.3
    request.setSslConfiguration(sslConfiguration);

    // Set some header info
    request.setRawHeader(QByteArray("de.securepoint.product"), QByteArray("sslvpnv2"));
    request.setRawHeader(QByteArray("de.securepoint.product.version"), FrmMain::instance()->version.toLatin1());
    request.setRawHeader(QByteArray("de.securepoint.product.os"), Utils::windowsVersion().toLatin1());
    request.setRawHeader(QByteArray("de.securepoint.product.arch"), (Utils::isX64Platform() ? QByteArray("x64") : QByteArray("x86")));

    // Set the URL    
    request.setUrl(QUrl("https://updatevpn.securepoint.de/sslvpn-client/LATEST"));
    this->manager->get(request);
}


