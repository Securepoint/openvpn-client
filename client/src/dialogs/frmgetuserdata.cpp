#include "frmgetuserdata.h"
#include "ui_frmgetuserdata.h"
#include <QtGui>
#include <QtGui/qguiapplication.h>
#include <QtWidgets/QDesktopWidget>

#include <service/srvcli.h>

#include <message.h>

#include <map>


float windowsDpiScale();

FrmGetUserData::FrmGetUserData(InputType::UserInputType type, const QString &name, int id, QWidget * parent)
    : FramelessDialog(parent),
    ui(new Ui::FrmGetUserData),
    dataAvail(false),
    frmType (type),
    vpnId (id),
    force (false)
{
   
    // 0 - Username
    // 1 - Pwd
    // 2 - OTP
    // 3 - PKCS12
    // 4 - Private Key für Crypted User Data

    ui->setupUi((QDialog*)this->getWrapperWidget());  

    QMetaObject::connectSlotsByName(this);

    auto geom = this->getWrapperWidget()->geometry();
    auto size = geom.size() * windowsDpiScale();
    geom = geometry();
    geom.setSize(size);
    this->getWrapperWidget()->setGeometry(geom);

    this->setupFrameless();

    ui->cbSaveData->setChecked(false);

    ui->groupBox->setTitle(QApplication::translate("FrmGetUserData", "Connection to"));
    ui->groupBox->setTitle(ui->groupBox->title() +  QLatin1String(": ") + name);


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

        SrvCLI::instance()->send(QLatin1String("CLOSE"), QString("%1").arg(this->vpnId));

        // Default senden
        if (frmType == InputType::PrivateKey) {
            if (Message::confirm(QObject::tr("The private key will be set to default <insecure>!"))) {
                emit cryptKey (QString("insecure"));
            }
        } else if (frmType == InputType::Username) {
            SrvCLI::instance()->send(QLatin1String("UNEEDED"), QString("%1;").arg(this->vpnId));
        } else if (frmType == InputType::Password) {
            SrvCLI::instance()->send(QLatin1String("PWDNEEDED"), QString("%1;").arg(this->vpnId));
        } else if (frmType == InputType::Pkcs12) {
            SrvCLI::instance()->send(QLatin1String("PKNEEDED"), QString("%1;").arg(this->vpnId));
        } else if (frmType == InputType::Otp) {
            SrvCLI::instance()->send(QLatin1String("CKNEEDED"), QString("%1;").arg(this->vpnId));
        } else if (frmType == InputType::HttpUsername) {
            SrvCLI::instance()->send(QLatin1String("HTTPUSERNEEDED"), QString("%1;").arg(this->vpnId));
        } else if (frmType == InputType::HttpPassword) {
            SrvCLI::instance()->send(QLatin1String("HTTPPASSNEEDED"), QString("%1;").arg(this->vpnId));
        } else if(frmType == InputType::CryptPassword) {
            emit cryptKey (QString(""));
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
    } else if(this->frmType == InputType::Pkcs12) {
        ui->lblDescription->setText(QObject::tr("PKCS12:"));
        ui->txtDataField->setEchoMode(QLineEdit::Password);
        ui->cbSaveData->setEnabled(true);
    } else {
        ui->lblDescription->setText(QObject::tr("Password:"));
        ui->txtDataField->setEchoMode(QLineEdit::Password);
        ui->cbSaveData->setEnabled(false);
    }

    int winW = this->width();
    int winH = this->height();

    int left (0);
    int top (0);

    {
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

    // Öffnen
    e->accept();
    this->setWindowState(Qt::WindowActive);
    ui->txtDataField->setFocus(Qt::PopupFocusReason);
}

void FrmGetUserData::on_cmdClose_clicked()
{
    this->close();
}

void FrmGetUserData::on_cmdClose_2_clicked()
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
        SrvCLI::instance()->send(QLatin1String("UNEEDED"), QString("%1;%2").arg(this->vpnId).arg(ui->txtDataField->text()));
        emit saveUserData(this->vpnId, 0, ui->txtDataField->text(), ui->cbSaveData->isChecked());
        this->close();
    } else if (frmType == InputType::Password) {
        SrvCLI::instance()->send(QLatin1String("PWDNEEDED"), QString("%1;%2").arg(this->vpnId).arg(ui->txtDataField->text()));
        emit saveUserData(this->vpnId, 1, ui->txtDataField->text(), ui->cbSaveData->isChecked());
        this->close();
    } else if (frmType == InputType::Pkcs12) {
        SrvCLI::instance()->send(QLatin1String("PKNEEDED"), QString("%1;%2").arg(this->vpnId).arg(ui->txtDataField->text()));
        emit saveUserData(this->vpnId, 3, ui->txtDataField->text(), ui->cbSaveData->isChecked());
        this->close();
    } else if (frmType == InputType::HttpUsername) {
        SrvCLI::instance()->send(QLatin1String("HTTPUSERNEEDED"), QString("%1;%2").arg(this->vpnId).arg(ui->txtDataField->text()));
        emit saveUserData(this->vpnId, 5, ui->txtDataField->text(), ui->cbSaveData->isChecked());
        this->close();
    } else if (frmType == InputType::HttpPassword) {
        SrvCLI::instance()->send(QLatin1String("HTTPPASSNEEDED"), QString("%1;%2").arg(this->vpnId).arg(ui->txtDataField->text()));
        emit saveUserData(this->vpnId, 6, ui->txtDataField->text(), ui->cbSaveData->isChecked());
        this->close();
    } else if (frmType == InputType::Otp) {
        SrvCLI::instance()->send(QLatin1String("CKNEEDED"), QString("%1;%2").arg(this->vpnId).arg(ui->txtDataField->text()));
        emit saveUserData(this->vpnId, 2, ui->txtDataField->text(), ui->cbSaveData->isChecked());
        this->close();
    } else if (frmType == InputType::PrivateKey) {
        emit cryptKey(ui->txtDataField->text());
        this->close();
    } else if (frmType == InputType::CryptPassword) {
        emit cryptKey(ui->txtDataField->text());
        this->close();
    }
}
