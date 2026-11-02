#include "clientsettingswidget.h"
#include "ui_clientsettingswidget.h"

#include <frmmain.h>
#include <widgetfactory.h>
#include <QtGui/qpainter.h>
#include "settings.h"
#include <utils.h>
#include <service/SrvCLI.h>
#include <debug/debug.h>

#include <message.h>
#include <proxysettings.h>

#include <QtNetwork/QHostAddress>
#include <QRegularExpression>
#include <QUrl>

extern bool g_bPortable;

// Validates a proxy host (IPv4, IPv6 or host name) to prevent OpenVPN argument injection.
// On success the normalized value is written to *normalized.
static bool validHostOrIp(const QString &input, QString *normalized)
{
    // Reject empty input and leading/trailing whitespace.
    if (!normalized || input.isEmpty() || input != input.trimmed())
        return false;

    for (QChar c : input) {
        // Whitespace and control characters would split or break the command line.
        if (c.isSpace() || c.unicode() < 0x20 || c.unicode() == 0x7f)
            return false;
        // Reject bracketed IPv6 ("[::1]") and IPv6 zone IDs ("fe80::1%eth0").
        if (c == QLatin1Char('[') || c == QLatin1Char(']') || c == QLatin1Char('%'))
            return false;
    }

    // Accept a fully qualified name with trailing dot ("proxy.example.com.") by dropping the dot.
    QString host = input;
    if (host.endsWith(QLatin1Char('.')))
        host.chop(1);

    // First try to parse the input as an IP address.
    QHostAddress ip;
    if (ip.setAddress(host)) {
        // Only the canonical dotted quad a.b.c.d is accepted; short forms like "127.1" or "8080" are rejected.
        if (ip.protocol() == QAbstractSocket::IPv4Protocol && ip.toString() != host)
            return false;
        *normalized = ip.toString();
        return true;
    }

    // Otherwise treat it as a host name; convert IDN (e.g. umlauts) to ASCII punycode. Max. length is 253 (RFC 1035).
    const QByteArray ace = QUrl::toAce(host);
    if (ace.isEmpty() || ace.size() > 253)
        return false;

    // RFC 952/1123 host names: letters, digits and '-' only; '_' is intentionally rejected.
    static const QRegularExpression label(
        QStringLiteral("\\A[A-Za-z0-9](?:[A-Za-z0-9-]{0,61}[A-Za-z0-9])?\\z"));
    static const QRegularExpression numericOnly(QStringLiteral("\\A[0-9]+\\z"));

    // Each label must be 1-63 chars and must not start or end with '-' (blocks "--option" injection).
    const QList<QByteArray> parts = ace.split('.');
    for (const QByteArray &part : parts) {
        if (part.size() > 63 || !label.match(QString::fromLatin1(part)).hasMatch())
            return false;
    }

    // An all-numeric last label is no valid host name (e.g. "127.1" or "8080" not parsed as IPv4).
    if (numericOnly.match(QString::fromLatin1(parts.last())).hasMatch())
        return false;

    *normalized = QString::fromLatin1(ace);
    return true;
}

// Validates a proxy port: ASCII digits only, range 1-65535.
// On success the normalized value (e.g. "08080" -> "8080") is written to *normalized.
static bool validPort(const QString &input, QString *normalized)
{
    // Reject empty input and leading/trailing whitespace.
    if (!normalized || input.isEmpty() || input != input.trimmed())
        return false;

    // Only ASCII digits; rejects signs, spaces and non-ASCII digits.
    for (QChar c : input) {
        if (!c.isDigit() || c.unicode() > 0x7f)
            return false;
    }

    bool ok = false;
    const uint port = input.toUInt(&ok);
    if (!ok || port < 1 || port > 65535)
        return false;

    // Store the number itself, never the raw input.
    *normalized = QString::number(port);
    return true;
}

ClientSettingsWidget::ClientSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientSettingsWidget)
{
    ui->setupUi(this);

    this->tapCount = 0;

    //
    ui->tabGeneral->setLayout(ui->verticalLayoutGeneral);
    //
    //ui->tabConnection->setLayout(ui->verticalLayoutConnection);
    //
    //ui->gbMaschineConfigsDirectory->setLayout(ui->verticalLayoutMaschineConfigDirectory);
}

ClientSettingsWidget::~ClientSettingsWidget()
{
    delete ui;
}

void ClientSettingsWidget::on_cmdBack_clicked()
{
    //
    // Close me
    //
    // Save the proxy settings

    if (ui->rbUseConfig->isChecked()) {
        ProxySettings::instance()->setProxyTypeInUse("CONFIG");
    } else if (ui->rbUseIE->isChecked()) {
        ProxySettings::instance()->setProxyTypeInUse("IE");
    } else {
        QString host;
        QString port;
        if (!validHostOrIp(ui->txtProxyIP->text(), &host)) {
            Message::error(QObject::tr("Invalid proxy address. Only IP addresses or host names are allowed."), QObject::tr("Proxy settings"), this);
            return;
        }
        if (!validPort(ui->txtProxyPort->text(), &port)) {
            Message::error(QObject::tr("Invalid proxy port. Only numbers between 1 and 65535 are allowed."), QObject::tr("Proxy settings"), this);
            return;
        }

        ProxySettings::instance()->setProxyTypeInUse("MANUAL");
        ProxySettings::instance()->setIsHttpProxy(ui->rbHttpProxy->isChecked());
        //
        ProxySettings::instance()->setProxyHost(host);
        ProxySettings::instance()->setProxyPort(port);
    }

    FrmMain::instance()->mainWidget()->showWidget(MainView);
}

void ClientSettingsWidget::on_rbUseManual_toggled(bool checked)
{
    if (checked) {
        ui->rbHttpProxy->setEnabled(true);
        ui->rbSocksProxy->setEnabled(true);
        ui->txtProxyIP->setEnabled(true);
        ui->txtProxyPort->setEnabled(true);
    } else {
        ui->rbHttpProxy->setEnabled(false);
        ui->rbSocksProxy->setEnabled(false);
        ui->txtProxyIP->setEnabled(false);
        ui->txtProxyPort->setEnabled(false);
    }
}

void ClientSettingsWidget::paintEvent(QPaintEvent *pe)
{
    //
    // Draw it with stylesheet support
    // Sometimes I hate Qt!
    // For further information, take a look at
    // http://stackoverflow.com/questions/18344135/why-do-stylesheets-not-work-when-subclassing-qwidget-and-using-q-object
    //

    Q_UNUSED(pe)

    QStyleOption option;
    option.initFrom(this);
    //
    QPainter painter(this);
    //
    style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);
}

void ClientSettingsWidget::showEvent(QShowEvent *event)
{
    //
    // Init the values
    //

    // General
    ui->cbStartOnWindows->setChecked(Settings::instance()->startOnWindows());
    ui->cbBlockShutdown->setChecked(Settings::instance()->blockShutdown());
    //ui->cbShowBallon->setChecked(Settings::instance()->showBallon());
    ui->cbAlwaysPopup->setChecked(Settings::instance()->alwaysPopup());
    //
    ui->cbShowSmallDhKeyHInt->setChecked(Settings::instance()->dhShowSmallKeyInformation());


    ui->cbSendToTrayAfterConnect->setChecked(Settings::instance()->sendToTrayAfterConnect());
    // Connection
    ui->cbInteract->setChecked(Settings::instance()->useInteract());
    //ui->cbAutoRestart->setChecked(Settings::instance()->autoRestart());

    //ui->cbAutoRestart->setVisible(false);
    //ui->cbInteract->setVisible(false);

    //
    //ui->txtMaschineConfigDirectory->setText(Settings::instance()->maschineConfigDirectory());

    // Maschine configurations is only visible when the user has administraion permissions
    //ui->gbMaschineConfigsDirectory->setVisible(Utils::isUserAdmin());

    // Set general as default
    ui->tabWidget->setCurrentIndex(0);

    SrvCLI::instance()->send(QLatin1String("TAPCOUNT"), QLatin1String(""));
   
    ui->lbTAPCount->setText(QObject::tr("Current TAP Count: ") + QString("%1").arg(tapCount));


    if (!ProxySettings::instance()->settingsAvailable()) {
        ui->rbUseConfig->setChecked(true);
        ui->rbUseIE->setEnabled(true);
        ui->rbUseManual->setEnabled(true);
        ui->rbHttpProxy->setEnabled(false);
        ui->rbSocksProxy->setEnabled(false);
        ui->txtProxyIP->setEnabled(false);
        ui->txtProxyPort->setEnabled(false);
    } else {

        if (ProxySettings::instance()->proxyTypeInUse() == "CONFIG") {
            ui->rbUseConfig->setChecked(true);
        } else if (ProxySettings::instance()->proxyTypeInUse() == "IE") {
            ui->rbUseIE->setChecked(true);
        } else {
            // MANUAL
            ui->rbHttpProxy->setEnabled(true);
            ui->rbSocksProxy->setEnabled(true);
            ui->txtProxyIP->setEnabled(true);
            ui->txtProxyPort->setEnabled(true);
            ui->rbUseManual->setChecked(true);
            //
            ui->txtProxyPort->setText(ProxySettings::instance()->proxyPort().isEmpty() ? "8080" : ProxySettings::instance()->proxyPort());
            ui->txtProxyIP->setText(ProxySettings::instance()->proxyHost());
            if (ProxySettings::instance()->isHttpProxy())
                ui->rbHttpProxy->setChecked(true);
            else
                ui->rbSocksProxy->setChecked(true);

        }

        ui->rbUseIE->setEnabled(true);

    }
    //
    event->accept();
}

void ClientSettingsWidget::receivedTapCount(int count)
{
    this->tapCount = count;

    ui->lbTAPCount->setText(QObject::tr("Current TAP Count: ") + QString("%1").arg(count));
}

void ClientSettingsWidget::on_cbStartOnWindows_toggled(bool checked)
{
    Q_UNUSED(checked)
}

void ClientSettingsWidget::on_cbBlockShutdown_toggled(bool checked)
{
    //
    // Update setting
    //

    Settings::instance()->setBlockShutdown(checked);
}

void ClientSettingsWidget::on_cbAlwaysPopup_toggled(bool checked)
{
    //
    // Update setting
    //

    Settings::instance()->setAlwaysPopup(checked);
}

void ClientSettingsWidget::on_cbInteract_toggled(bool checked)
{
    //
    // Update setting
    //

    Settings::instance()->setUseInteract(checked);
}

void ClientSettingsWidget::on_cmdAddTap_clicked()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    SrvCLI::instance()->send(QLatin1String("ADDTAP"), QLatin1String(""));
}

void ClientSettingsWidget::on_cmdRemoveTap_clicked()
{
      if(Message::confirm(QObject::tr("Do you realy want to remove all TAP device?"))) {
            // Befehl an den Dienst senden
            QApplication::setOverrideCursor(Qt::WaitCursor);
            SrvCLI::instance()->send(QLatin1String("REMOVETAP"), QLatin1String(""));
        }
}

void ClientSettingsWidget::on_cbShowSmallDhKeyHInt_toggled(bool checked)
{
   Settings::instance()->setDhShowSmallKeyInformation(checked);
}

void ClientSettingsWidget::on_cbStartOnWindows_clicked(bool checked)
{
    //
    // Update setting
    //

    Settings::instance()->setStartOnWindows(checked);
    Settings::instance()->setRegEntryStartOnWindows(checked);
}

void ClientSettingsWidget::on_cbSendToTrayAfterConnect_toggled(bool checked)
{
    Settings::instance()->setSendToTrayAfterConnect(checked);
}
