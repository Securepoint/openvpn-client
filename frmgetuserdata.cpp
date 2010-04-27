#include "frmgetuserdata.h"
#include "ui_frmgetuserdata.h"
#include "settings.h"
#include <QtGui>

FrmGetUserData *FrmGetUserData::mInst = NULL;

FrmGetUserData *FrmGetUserData::getInstance() {
    if (!mInst)
        mInst = new FrmGetUserData ();
    return mInst;
}

FrmGetUserData::FrmGetUserData() :
    QDialog(),
    ui(new Ui::FrmGetUserData)
{
    ui->setupUi(this);
    this->dataAvail = false;
    // 0 - Username
    // 1 - Pwd
    // 2 - OTP
    // 3 - PKCS12
    // 4 - Private Key für Crypted User Data
    this->frmType = 0;
    this->dataField = "";
    ui->cbSaveData->setChecked(false);
    ui->cbSaveData->setEnabled(false);
    this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint);
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

void FrmGetUserData::showEvent(QShowEvent *e) {
    this->dataAvail = false;
    this->dataField = "";
    ui->txtDataField->setText("");
    if (this->frmType == 0) {
        ui->lblDescription->setText(tr("Username:"));
        ui->txtDataField->setEchoMode(QLineEdit::Normal);
        if (Settings::getInstance()->getUseCryptedData()) {
            ui->cbSaveData->setChecked(true);
            ui->cbSaveData->setEnabled(true);
        } else {
            ui->cbSaveData->setChecked(false);
            ui->cbSaveData->setEnabled(false);
        }
    } else if (this->frmType == 1) {
        ui->lblDescription->setText(tr("Password:"));
        ui->txtDataField->setEchoMode(QLineEdit::Password);
        if (Settings::getInstance()->getUseCryptedData()) {
            ui->cbSaveData->setChecked(true);
            ui->cbSaveData->setEnabled(true);
        } else {
            ui->cbSaveData->setChecked(false);
            ui->cbSaveData->setEnabled(false);
        }
    } else if (this->frmType == 2) {
        ui->lblDescription->setText(tr("One Time Pad:"));
        ui->txtDataField->setEchoMode(QLineEdit::Password);
        ui->cbSaveData->setChecked(false);
        ui->cbSaveData->setEnabled(false);
    } else if (this->frmType == 4) {
        ui->lblDescription->setText(tr("Crypt Key:"));
        ui->txtDataField->setEchoMode(QLineEdit::Password);
        ui->cbSaveData->setChecked(false);
        ui->cbSaveData->setEnabled(false);
    } else {
        ui->lblDescription->setText(tr("PKCS12:"));
        ui->txtDataField->setEchoMode(QLineEdit::Password);
        ui->cbSaveData->setChecked(false);
        ui->cbSaveData->setEnabled(false);
    }
    // Mittig ausrichten
    int screenH = qApp->desktop()->height();
    int screenW = qApp->desktop()->width();
    int winH = 140;
    int winW = 330;
    // Nun die neuen setzen
    this->setGeometry((screenW / 2) - (winW / 2), (screenH / 2) - (winH / 2), winW, winH);
    // Öffnen
    e->accept();
    this->setWindowState(Qt::WindowActive);
    ui->txtDataField->setFocus(Qt::PopupFocusReason);
}

void FrmGetUserData::on_cmdClose_clicked()
{
    this->dataField = "";
    if (this->frmType == 4) {
        // Bei Crypt darf kein leerer String zurückgegeben werden,
        // da die CryptFunktion nicht mit leeren Key ver- und entschlüsseln kann
        QMessageBox msgBox;
                    msgBox.setWindowTitle(tr("Securepoint SSL VPN"));
                    msgBox.setWindowIcon(QIcon(":/images/logo.png"));
                    msgBox.setText(tr("Private Key"));
                    msgBox.setInformativeText(tr("The private key will be set to default <insecure>!"));
                    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
                    msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
        int ret = msgBox.exec();
        if (ret == QMessageBox::Ok) {
            this->dataField = "insecure";
            this->dataAvail = true;
            emit dataIsAvailable();
            this->close();
        }
        // Cancel gedrückt, Dialog kann noch geändert werden
    } else {
        this->dataAvail = true;
        emit dataIsAvailable();
        this->close();
    }
}

void FrmGetUserData::on_cmdOK_clicked()
{
    if (this->frmType == 4) {
        if (ui->txtDataField->text() == "") {
            QMessageBox msgBox;
                        msgBox.setWindowTitle(tr("Securepoint SSL VPN"));
                        msgBox.setWindowIcon(QIcon(":/images/logo.png"));
                        msgBox.setText(tr("Private Key"));
                        msgBox.setInformativeText(tr("It is not possible to use an empty private key!"));
                        msgBox.setStandardButtons(QMessageBox::Ok);
                        msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
            msgBox.exec();
        } else {
            // Nicht leer alles ok
            this->dataField = ui->txtDataField->text();
            this->dataAvail = true;
            emit dataIsAvailable();
            this->close();
        }
    } else {
        this->dataField = ui->txtDataField->text();
        this->dataAvail = true;
        emit dataIsAvailable();
        this->close();
    }
}

bool FrmGetUserData::isDataAvailable() {
    return this->dataAvail;
}

QString FrmGetUserData::getDataField() {
    return this->dataField;
}

void FrmGetUserData::setFrmType(int type) {
    this->frmType = type;
}

bool FrmGetUserData::isSaveChecked() {
    return ui->cbSaveData->isChecked();
}

void FrmGetUserData::closeMe() {
    //this->hide();
}
