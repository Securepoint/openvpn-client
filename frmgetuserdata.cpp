#include "frmgetuserdata.h"
#include "ui_frmgetuserdata.h"
#include "settings.h"
#include <QtGui>

#include "message.h"
#include "preferences.h"

#include "network/srvcli.h"

FrmGetUserData::FrmGetUserData(InputType::UserInputType type, int id)
    : QDialog(),
      ui(new Ui::FrmGetUserData),
      dataAvail(false),
      frmType (type),
      vpnId (id),
      force (false)
{
    ui->setupUi(this);

    // 0 - Username
    // 1 - Pwd
    // 2 - OTP
    // 3 - PKCS12
    // 4 - Private Key für Crypted User Data

    ui->cbSaveData->setChecked(false);
    this->setWindowFlags(Qt::WindowCloseButtonHint);
}

FrmGetUserData::~FrmGetUserData()
{
    delete ui;
}

void FrmGetUserData::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void FrmGetUserData::receivedCloseMe()
{
    force = true;
    this->close();
}

void FrmGetUserData::closeEvent(QCloseEvent *e)
{        
    if (!force && !this->dataAvail) {
        // Default senden
        if (frmType == InputType::PrivateKey) {
            if (Message::confirm(QObject::tr("The private key will be set to default <insecure>!"))) {
                emit cryptKey (QString("insecure"));
            }
        } else if (frmType == InputType::Username) {
            if (Settings::getInstance()->getIsPortableClient()) {
                emit writeUserData("");
            } else {
                SrvCLI::instance()->send(QLatin1String("UNEEDED"), QString("%1;").arg(this->vpnId));
            }
        } else if (frmType == InputType::Password) {
            if (Settings::getInstance()->getIsPortableClient()) {
                emit writeUserData("");
            } else {
                SrvCLI::instance()->send(QLatin1String("PWDNEEDED"), QString("%1;").arg(this->vpnId));
            }
        } else if (frmType == InputType::Pkcs12) {
            if (Settings::getInstance()->getIsPortableClient()) {
                emit writeUserData("");
            } else {
                SrvCLI::instance()->send(QLatin1String("PKNEEDED"), QString("%1;").arg(this->vpnId));
            }
        } else if (frmType == InputType::Otp) {
            if (Settings::getInstance()->getIsPortableClient()) {
                emit writeUserData("");
            } else {
                SrvCLI::instance()->send(QLatin1String("CKNEEDED"), QString("%1;").arg(this->vpnId));
            }
        } else if (frmType == InputType::HttpUsername) {
            if (Settings::getInstance()->getIsPortableClient()) {
                emit writeUserData("");
            } else {
                SrvCLI::instance()->send(QLatin1String("HTTPUSERNEEDED"), QString("%1;").arg(this->vpnId));
            }
        } else if (frmType == InputType::HttpPassword) {
            if (Settings::getInstance()->getIsPortableClient()) {
                emit writeUserData("");
            } else {
                SrvCLI::instance()->send(QLatin1String("HTTPPASSNEEDED"), QString("%1;").arg(this->vpnId));
            }
        }
    }

    e->accept();
}

void FrmGetUserData::showEvent(QShowEvent *e) {        
    ui->txtDataField->setText("");
    if (this->frmType == InputType::Username) {
        ui->lblDescription->setText(QObject::tr("Username:"));
        ui->txtDataField->setEchoMode(QLineEdit::Normal);
        ui->cbSaveData->setEnabled(true);
    } else if (this->frmType == InputType::Password) {
        ui->lblDescription->setText(QObject::tr("Password:"));
        ui->txtDataField->setEchoMode(QLineEdit::Password);        
        ui->cbSaveData->setEnabled(true);
    } else if (this->frmType == InputType::Otp) {
        ui->lblDescription->setText(QObject::tr("One Time Pad:"));
        ui->txtDataField->setEchoMode(QLineEdit::Password);        
        ui->cbSaveData->setEnabled(false);
    } else if (this->frmType == InputType::PrivateKey) {
        ui->lblDescription->setText(QObject::tr("Crypt Key:"));
        ui->txtDataField->setEchoMode(QLineEdit::Password);        
        ui->cbSaveData->setEnabled(false);
    } else if (this->frmType == InputType::HttpUsername) {
        ui->lblDescription->setText(QObject::tr("Http user:"));
        ui->txtDataField->setEchoMode(QLineEdit::Normal);
        ui->cbSaveData->setEnabled(true);
    } else if (this->frmType == InputType::HttpPassword) {
        ui->lblDescription->setText(QObject::tr("Http password:"));
        ui->txtDataField->setEchoMode(QLineEdit::Password);
        ui->cbSaveData->setEnabled(true);
    } else {
        ui->lblDescription->setText(QObject::tr("PKCS12:"));
        ui->txtDataField->setEchoMode(QLineEdit::Password);        
        ui->cbSaveData->setEnabled(true);
    }

    int winW = this->width();
    int winH = this->height();

    int left (0);
    if (Preferences::instance()->isVisible()) {
        // Wenn das Hauptfenster offen ist mittig über diesem plazieren
        left = Preferences::instance()->geometry().x();
        left = left + (Preferences::instance()->geometry().width() - winW) / 2;
    } else {
        // Desktop auswerten
        left = qApp->desktop()->width();
        // Die Breite bei virtuellen Desktops vierteln
        if (left > 2000 && qApp->desktop()->isVirtualDesktop()) {
            left /= 4;
        }
    }
    // Nun die neuen setzen
    this->setGeometry(left, (qApp->desktop()->height() / 2) - (winH / 2), winW, winH);

    // Öffnen
    e->accept();
    this->setWindowState(Qt::WindowActive);
    ui->txtDataField->setFocus(Qt::PopupFocusReason);
}

void FrmGetUserData::on_cmdClose_clicked()
{        
    this->close();
}

void FrmGetUserData::on_cmdOK_clicked()
{    
    if (frmType == InputType::PrivateKey) {
        if (ui->txtDataField->text().isEmpty()) {
            Message::error(QObject::tr("It is not possible to use an empty private key!"), QObject::tr("Private Key"));
            return;
        }
    }

    // 0 - Username
    // 1 - Pwd
    // 2 - OTP
    // 3 - PKCS12
    // 4 - Private Key für Crypted User Data
    this->dataAvail = true;
    if (frmType == InputType::Username) {
        if (Settings::getInstance()->getIsPortableClient()) {
            emit writeUserData(ui->txtDataField->text());
        } else {
            SrvCLI::instance()->send(QLatin1String("UNEEDED"), QString("%1;%2").arg(this->vpnId).arg(ui->txtDataField->text()));
        }
        emit saveUserData(this->vpnId, 0, ui->txtDataField->text(), ui->cbSaveData->isChecked());
        this->close();
    } else if (frmType == InputType::Password) {
        if (Settings::getInstance()->getIsPortableClient()) {
            emit writeUserData(ui->txtDataField->text());
        } else {
            SrvCLI::instance()->send(QLatin1String("PWDNEEDED"), QString("%1;%2").arg(this->vpnId).arg(ui->txtDataField->text()));
        }
        emit saveUserData(this->vpnId, 1, ui->txtDataField->text(), ui->cbSaveData->isChecked());
        this->close();
    } else if (frmType == InputType::Pkcs12) {
        if (Settings::getInstance()->getIsPortableClient()) {
            emit writeUserData(ui->txtDataField->text());
        } else {
            SrvCLI::instance()->send(QLatin1String("PKNEEDED"), QString("%1;%2").arg(this->vpnId).arg(ui->txtDataField->text()));
        }
        emit saveUserData(this->vpnId, 3, ui->txtDataField->text(), ui->cbSaveData->isChecked());
        this->close();
    } else if (frmType == InputType::HttpUsername) {
        if (Settings::getInstance()->getIsPortableClient()) {
            emit writeUserData(ui->txtDataField->text());
        } else {
            SrvCLI::instance()->send(QLatin1String("HTTPUSERNEEDED"), QString("%1;%2").arg(this->vpnId).arg(ui->txtDataField->text()));
        }
        emit saveUserData(this->vpnId, 5, ui->txtDataField->text(), ui->cbSaveData->isChecked());
        this->close();
    } else if (frmType == InputType::HttpPassword) {
        if (Settings::getInstance()->getIsPortableClient()) {
            emit writeUserData(ui->txtDataField->text());
        } else {
            SrvCLI::instance()->send(QLatin1String("HTTPPASSNEEDED"), QString("%1;%2").arg(this->vpnId).arg(ui->txtDataField->text()));
        }
        emit saveUserData(this->vpnId, 6, ui->txtDataField->text(), ui->cbSaveData->isChecked());
        this->close();
    } else if (frmType == InputType::Otp) {
        if (Settings::getInstance()->getIsPortableClient()) {
            emit writeUserData(ui->txtDataField->text());
        } else {
            SrvCLI::instance()->send(QLatin1String("CKNEEDED"), QString("%1;%2").arg(this->vpnId).arg(ui->txtDataField->text()));
        }
        emit saveUserData(this->vpnId, 2, ui->txtDataField->text(), ui->cbSaveData->isChecked());
        this->close();
    } else if (frmType == InputType::PrivateKey) {
        emit cryptKey(ui->txtDataField->text());
        this->close();
    }
}
