#include "frmupdatesettings.h"
#include "ui_frmupdatesettings.h"
#include "preferences.h"

#include "settings.h"
#include "message.h"

FrmUpdateSettings::FrmUpdateSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmUpdateSettings)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Tool);
}

FrmUpdateSettings::~FrmUpdateSettings()
{
    delete ui;
}

void FrmUpdateSettings::showEvent(QShowEvent *e)
{
    //
    // Init the dialog
    //

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
    //
    this->setMaximumHeight(winH);
    this->setMinimumHeight(winH);
    //
    this->setMaximumWidth(winW);
    this->setMinimumWidth(winW);
    // Öffnen
    e->accept();
    this->setWindowState(Qt::WindowActive);

    ui->cbAutoUpdate->setChecked(Settings::getInstance()->isAutoUpdate());
    //
    ui->txtSource->setText(Settings::getInstance()->updateSource());
    ui->txtSource->setEnabled(false);

    //
    if (Settings::getInstance()->updateUseProxy()) {
        ui->cbUseProxy->setChecked(true);
        ui->txtpPort->setEnabled(true);
        ui->txtIp->setEnabled(true);
        ui->txtProxyUser->setEnabled(true);
        ui->txtProxyPassword->setEnabled(true);
    } else {
        ui->cbDirectConnection->setChecked(true);
        ui->txtpPort->setEnabled(false);
        ui->txtIp->setEnabled(false);
        ui->txtProxyUser->setEnabled(false);
        ui->txtProxyPassword->setEnabled(false);
    }

    ui->txtIp->setText(Settings::getInstance()->updateProxyIp());
    ui->txtpPort->setText(Settings::getInstance()->updateProxyPort());
    //
    ui->txtProxyUser->setText(Settings::getInstance()->updateProxyUser());
    ui->txtProxyPassword->setText(Settings::getInstance()->updateProxyPassword());
    //
    if (Settings::getInstance()->useSourceForge()) {
        ui->rbUseSourceForge->setChecked(true);
    } else {
        ui->rbUseSecurepoint->setChecked(true);
    }

    //
    e->accept();
}

void FrmUpdateSettings::on_cmdCancel_clicked()
{
    this->close();
}

void FrmUpdateSettings::on_cmdSave_clicked()
{
    if (ui->cbUseProxy->isChecked()) {
        if (ui->txtpPort->text().isEmpty() || ui->txtIp->text().isEmpty()) {
            Message::warning(QObject::tr("Proxy IP or port is empty!"));
            return;
        }
    }

    Settings::getInstance()->setAutoUpdate(ui->cbAutoUpdate->isChecked());
    if (!ui->txtSource->text().isEmpty()) {
        if (Settings::getInstance()->updateSource() != ui->txtSource->text()) {
            Settings::getInstance()->setUpdateSource(ui->txtSource->text());
        }
    }

    Settings::getInstance()->setUpdateUseProxy(ui->cbUseProxy->isChecked());

    Settings::getInstance()->setUpdateProxyIp(ui->txtIp->text());
    Settings::getInstance()->setUpdateProxyPort(ui->txtpPort->text());
    //
    Settings::getInstance()->setUpdateProxyUser(ui->txtProxyUser->text());
    Settings::getInstance()->setUpdateProxyPassword(ui->txtProxyPassword->text());
    //
    Settings::getInstance()->setUseSourceForge(ui->rbUseSourceForge->isChecked());

    this->close();
}

void FrmUpdateSettings::on_cmdCangeSource_clicked()
{
    ui->txtSource->setEnabled(true);
}

void FrmUpdateSettings::on_cbDirectConnection_toggled(bool checked)
{
    ui->txtIp->setEnabled(!checked);
    ui->txtpPort->setEnabled(!checked);
    ui->txtProxyUser->setEnabled(!checked);
    ui->txtProxyPassword->setEnabled(!checked);
}

void FrmUpdateSettings::on_cbUseProxy_toggled(bool checked)
{
    ui->txtIp->setEnabled(checked);
    ui->txtpPort->setEnabled(checked);
    ui->txtProxyUser->setEnabled(checked);
    ui->txtProxyPassword->setEnabled(checked);
}
