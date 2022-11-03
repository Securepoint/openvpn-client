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

extern bool g_bPortable;

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
        ProxySettings::instance()->setProxyTypeInUse("MANUAL");
        ProxySettings::instance()->setIsHttpProxy(ui->rbHttpProxy->isChecked());
        //
        ProxySettings::instance()->setProxyHost(ui->txtProxyIP->text());
        ProxySettings::instance()->setProxyPort(ui->txtProxyPort->text());
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
    //ui->cbInteract->setChecked(Settings::instance()->useInteract());
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
