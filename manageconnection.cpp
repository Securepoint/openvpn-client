#include "manageconnection.h"
#include "ui_manageconnection.h"

ManageConnection *ManageConnection::mInst = NULL;

ManageConnection *ManageConnection::getInstance() {
    if (!mInst)
        mInst = new ManageConnection ();
    return mInst;
}

ManageConnection::ManageConnection() :
    QDialog(),
    ui(new Ui::ManageConnection)
{
    ui->setupUi(this);
    this->configObj = NULL;
    this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint);
}

void ManageConnection::changeEvent(QEvent *e)
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

void ManageConnection::showEvent(QShowEvent *e) {
    // Fenster init
    this->setWindowTitle(tr("Edit Connection - ") + this->configObj->configName);
    // Init Widgets
    ui->cmbChiper->clear();
    ui->cmbDev->clear();
    ui->cmbProto->clear();
    ui->cmbRouteMethod->clear();

    ui->cmbDev->insertItem(0, tr("Not defined"));
    ui->cmbDev->insertItem(1, "tun");
    ui->cmbDev->insertItem(2, "tap");

    ui->cmbProto->insertItem(0, tr("Not defined"));
    ui->cmbProto->insertItem(1, "TCP");
    ui->cmbProto->insertItem(2, "UDP");

    ui->cmbRouteMethod->insertItem(0, "Not defined");
    ui->cmbRouteMethod->insertItem(1, "Exe");
    ui->cmbRouteMethod->insertItem(2, "IPAPI");

    this->fillCipherCombo();

    this->lastDir = AppFunc::getInstance()->getAppSavePath();

    this->resetFields();

    // Fenster füllen
    ui->tbSettings->setCurrentIndex(0);
    this->fillFieldFromConfig();


    // Mittig ausrichten
    int screenH = qApp->desktop()->height();
    int screenW = qApp->desktop()->width();
    int winH = 670;
    int winW = 440;
    // Nun die neuen setzen
    this->setGeometry((screenW / 2) - (winW / 2), (screenH / 2) - (winH / 2), winW, winH);
    // Öffnen
    e->accept();
    this->setWindowState(Qt::WindowActive);
}

void ManageConnection::setOpenVpnObject(OpenVpn *obj) {
        this->configObj = obj;
}

void ManageConnection::on_cmdClose_clicked()
{
    this->close();
}

void ManageConnection::fillCipherCombo() {
    ui->cmbChiper->insertItem(0, "Standard");
    ui->cmbChiper->insertItem(1, "DES-CFB");
    ui->cmbChiper->insertItem(2, "DES-CBC");
    ui->cmbChiper->insertItem(3, "RC2-CBC");
    ui->cmbChiper->insertItem(4, "RC2-CFB");
    ui->cmbChiper->insertItem(5, "RC2-OFB");
    ui->cmbChiper->insertItem(6, "DES-EDE-CBC");
    ui->cmbChiper->insertItem(7, "DES-EDE3-CBC");
    ui->cmbChiper->insertItem(8, "DES-OFB");
    ui->cmbChiper->insertItem(9, "DES-EDE-CFB");
    ui->cmbChiper->insertItem(10, "DES-EDE3-CFB");
    ui->cmbChiper->insertItem(11, "DES-EDE-OFB");
    ui->cmbChiper->insertItem(12, "DES-EDE3-OFB");
    ui->cmbChiper->insertItem(13, "DESX-CBC");
    ui->cmbChiper->insertItem(14, "BF-CBC");
    ui->cmbChiper->insertItem(15, "BF-CFB");
    ui->cmbChiper->insertItem(16, "BF-OFB");
    ui->cmbChiper->insertItem(17, "RC2-40-CBC");
    ui->cmbChiper->insertItem(18, "CAST5-CBC");
    ui->cmbChiper->insertItem(19, "CAST5-CFB");
    ui->cmbChiper->insertItem(20, "CAST5-OFB");
    ui->cmbChiper->insertItem(21, "RC2-64-CBC");
    ui->cmbChiper->insertItem(22, "AES-128-CBC");
    ui->cmbChiper->insertItem(23, "AES-128-OFB");
    ui->cmbChiper->insertItem(24, "AES-128-CFB");
    ui->cmbChiper->insertItem(25, "AES-192-CBC");
    ui->cmbChiper->insertItem(26, "AES-192-OFB");
    ui->cmbChiper->insertItem(27, "AES-192-CFB");
    ui->cmbChiper->insertItem(28, "AES-256-CBC");
    ui->cmbChiper->insertItem(29, "AES-256-OFB");
    ui->cmbChiper->insertItem(30, "AES-256-CFB");
    ui->cmbChiper->insertItem(31, "AES-128-CFB1");
    ui->cmbChiper->insertItem(32, "AES-192-CFB1");
    ui->cmbChiper->insertItem(33, "AES-256-CFB1");
    ui->cmbChiper->insertItem(34, "AES-128-CFB8");
    ui->cmbChiper->insertItem(35, "AES-192-CFB8");
    ui->cmbChiper->insertItem(36, "AES-256-CFB8");
    ui->cmbChiper->insertItem(37, "DES-CFB1");
    ui->cmbChiper->insertItem(38, "DES-CFB8");
}

int ManageConnection::getIndexFromCipher(QString cipher) {
    if ( cipher == "DES-CFB")
        return 1;
    else if ( cipher == "DES-CBC")
        return 2;
    else if ( cipher == "RC2-CBC")
        return 3;
    else if ( cipher == "RC2-CFB")
        return 4;
    else if ( cipher == "RC2-OFB")
        return 5;
    else if ( cipher == "DES-EDE-CBC")
        return 6;
    else if ( cipher == "DES-EDE3-CBC")
        return 7;
    else if ( cipher == "DES-OFB")
        return 8;
    else if ( cipher == "DES-EDE-CFB")
        return 9;
    else if ( cipher == "DES-EDE3-CFB")
        return 10;
    else if ( cipher == "DES-EDE-OFB")
        return 11;
    else if ( cipher == "DES-EDE3-OFB")
        return 12;
    else if ( cipher == "DESX-CBC")
        return 13;
    else if ( cipher == "BF-CBC")
        return 14;
    else if ( cipher == "BF-CFB")
        return 15;
    else if ( cipher == "BF-OFB")
        return 16;
    else if ( cipher == "RC2-40-CBC")
        return 17;
    else if ( cipher == "CAST5-CBC")
        return 18;
    else if ( cipher == "CAST5-CFB")
        return 19;
    else if ( cipher == "CAST5-OFB")
        return 20;
    else if ( cipher == "RC2-64-CBC")
        return 21;
    else if ( cipher == "AES-128-CBC")
        return 22;
    else if ( cipher == "AES-128-OFB")
        return 23;
    else if ( cipher == "AES-128-CFB")
        return 24;
    else if ( cipher == "AES-192-CBC")
        return 25;
    else if ( cipher == "AES-192-OFB")
        return 26;
    else if ( cipher == "AES-192-CFB")
        return 27;
    else if ( cipher == "AES-256-CBC")
        return 28;
    else if ( cipher == "AES-256-OFB")
        return 29;
    else if ( cipher == "AES-256-CFB")
        return 30;
    else if ( cipher == "AES-128-CFB1")
        return 31;
    else if ( cipher == "AES-192-CFB1")
        return 32;
    else if ( cipher == "AES-256-CFB1")
        return 33;
    else if ( cipher == "AES-128-CFB8")
        return 34;
    else if ( cipher == "AES-192-CFB8")
        return 35;
    else if ( cipher == "AES-256-CFB8")
        return 36;
    else if ( cipher == "DES-CFB1")
        return 37;
    else if ( cipher == "DES-CFB8")
        return 38;
    else
        return 0;
}

void ManageConnection::resetFields() {
    ui->cbClient->setChecked(false);
    ui->cbFloat->setChecked(false);
    ui->cbCompLzo->setChecked(false);
    ui->cbNobind->setChecked(false);
    ui->cbPersistKey->setChecked(false);
    ui->cbPersistTun->setChecked(false);
    ui->cbNoCache->setChecked(false);
    ui->cbUserPass->setChecked(false);
    ui->cbRedirectGateway->setChecked(false);

    ui->cmbDev->setCurrentIndex(0);
    ui->txtMtu->setText("");

    ui->txtRemote->setText("");
    ui->txtRemotePort->setText("");
    ui->cmbProto->setCurrentIndex(0);

    ui->txtCA->setText("");
    ui->txtCert->setText("");
    ui->txtKey->setText("");
    ui->cbCertIsServer->setChecked(false);

    // Linux
    ui->txtLinuxUser->setText("");
    ui->txtLinuxGroup->setText("");

    // Windows

    ui->cbMsfix->setChecked(false);
    ui->txtFragment->setText("");
    ui->cmbRouteMethod->setCurrentIndex(0);
    ui->txtDevNode->setText("");

    // Advanced
    ui->txtVerbose->setText("");
    ui->txtRouteDelay->setText("");
    ui->txtMute->setText("");

    ui->cbRandomHost->setChecked(false);
    ui->cbResolveHostname->setChecked(false);

    ui->cbHTTPRetry->setChecked(false);
    ui->txtHTTPIP->setText("");
    ui->txtHTTPPort->setText("");

    ui->cbMuteWirelessWarning->setChecked(false);

    ui->cmbChiper->setCurrentIndex(0);

    ui->txtAfterConnect->setText("");
    ui->txtAfterDisconnect->setText("");
    ui->txtBeforeConnect->setText("");
    ui->txtBeforeDisconnect->setText("");
    ui->txtErrorConnect->setText("");

    ui->txtScriptACDelay->setText("5000");
}

void ManageConnection::fillFieldFromConfig() {
    // Load Config Data
    QString fileContent = QString("");
    QString cFile;

    cFile = this->configObj->configPath + QString("/") + this->configObj->configName + QString(".ovpn");
    QFile cf (cFile);

    if (!cf.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Securepoint SSL VPN"));
        msgBox.setText(tr("Load Configuration"));
        msgBox.setWindowIcon(QIcon(":/images/logo.png"));
        msgBox.setInformativeText(tr("Unable to read file!"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
        msgBox.exec();        
        return;
    }
    ui->txtCA->setText("");
    ui->txtCert->setText("");
    ui->txtKey->setText("");

    QTextStream in(&cf);
    bool isClientChecked = false;
    bool isCAChecked = false;
    bool isDevChecked = false;
    bool isMtuChecked = false;
    bool isFragmentChecked = false;
    bool isMsFixChecked = false;
    bool isProtoChecked = false;
    bool isFloatChecked = false;
    bool isRemoteChecked = false;
    bool isNobindChecked = false;
    bool isPersistKeyChecked = false;
    bool isPersistTunChecked = false;
    bool isCertChecked = false;
    bool isKeyChecked = false;
    bool isCompLzoChecked = false;
    bool isVerboseChecked = false;
    bool isMuteChecked = false;
    bool isNoCacheChecked = false;
    bool isUserPassChecked = false;
    bool isRouteMethodChecked = false;
    bool isRouteDelayChecked = false;
    bool isDevNodeChecked = false;
    bool isRandomHostChecked = false;
    bool isResolveHostChecked = false;
    bool isLinuxUserChecked = false;
    bool isLinuxGroupChecked = false;
    bool isHttpRetryChecked = false;
    bool isHttpProxyChecked = false;
    bool isWirelessMuteWarningChecked = false;
    bool isCipherChecked = false;
    bool isCertTyoeServerChecked = false;
    bool isRedirectGatewayChecked = false;

    while (!in.atEnd()) {
        QString line = in.readLine();
        if (!isClientChecked) {
                if (line.toUpper().trimmed() == "CLIENT") {
                    isClientChecked = true;
                    ui->cbClient->setChecked(true);
                } else {
                    ui->cbClient->setChecked(false);
                }
        }
        if (!isCAChecked) {
                if (line.left(2).toUpper().trimmed() == "CA") {
                    isCAChecked = true;
                    QStringList keyvalList = line.split(" ");
                    if (keyvalList.size() != 2)
                        ui->txtCA->setText("");
                    else {
                        QString val = keyvalList[1];
                        ui->txtCA->setText(val.replace("\"",""));
                    }
                }
            }
        if (!isDevChecked) {
                if (line.left(3).toUpper().trimmed() == "DEV") {
                    isDevChecked = true;
                    QStringList keyvalList = line.split(" ");
                    if (keyvalList.size() != 2)
                        ui->cmbDev->setCurrentIndex(0);
                    else {
                        QString val = keyvalList[1];
                        if (val.trimmed().toUpper() == "TUN")
                            ui->cmbDev->setCurrentIndex(1);
                        else
                            ui->cmbDev->setCurrentIndex(2);
                    }
                }
            }
        if (!isMtuChecked) {
                if (line.left(7).toUpper().trimmed() == "TUN-MTU") {
                    isMtuChecked = true;
                    QStringList keyvalList = line.split(" ");
                    if (keyvalList.size() != 2)
                        ui->txtMtu->setText("");
                    else {
                        QString val = keyvalList[1];
                        ui->txtMtu->setText(val.trimmed());
                    }
                }
            }
        if (!isFragmentChecked){
                if (line.left(8).toUpper().trimmed() == "FRAGMENT") {
                    isFragmentChecked = true;
                    QStringList keyvalList = line.split(" ");
                    if (keyvalList.size() != 2)
                        ui->txtFragment->setText("");
                    else {
                        QString val = keyvalList[1];
                        ui->txtFragment->setText(val.trimmed());
                    }
                }
            }
        if (!isMsFixChecked){
                if (line.toUpper().trimmed() == "MSSFIX") {
                    isMsFixChecked = true;
                    ui->cbMsfix->setChecked(true);
                } else {
                    ui->cbMsfix->setChecked(false);
                }
            }

        if (!isRedirectGatewayChecked){
                if (line.toUpper().trimmed() == "REDIRECT-GATEWAY") {
                    isRedirectGatewayChecked = true;
                    ui->cbRedirectGateway->setChecked(true);
                } else {
                    ui->cbRedirectGateway->setChecked(false);
                }
            }

        if (!isProtoChecked){
                if (line.left(5).toUpper().trimmed() == "PROTO") {
                    isProtoChecked = true;
                    QStringList keyvalList = line.split(" ");
                    if (keyvalList.size() != 2)
                        ui->cmbProto->setCurrentIndex(0);
                    else {
                        QString val = keyvalList[1];
                        if (val.trimmed().toUpper() == "TCP")
                            ui->cmbProto->setCurrentIndex(1);
                        else
                            ui->cmbProto->setCurrentIndex(2);
                    }
                }
            }
        if (!isFloatChecked){
                if (line.toUpper().trimmed() == "FLOAT") {
                    isFloatChecked = true;
                    ui->cbFloat->setChecked(true);
                } else {
                    ui->cbFloat->setChecked(false);
                }
            }
        if (!isRemoteChecked){
                if (line.left(6).toUpper().trimmed() == "REMOTE") {
                    isRemoteChecked = true;
                    QStringList keyvalList = line.split(" ");
                    if (keyvalList.size() != 3){
                        ui->txtRemotePort->setText("");
                        ui->txtRemote->setText("");
                    } else {
                        QString val = keyvalList[1];
                        QString valport = keyvalList[2];
                        ui->txtRemote->setText(val.replace("\"",""));
                        ui->txtRemotePort->setText(valport.replace("\"",""));
                    }
                }
            }
        if (!isNobindChecked){
                if (line.toUpper().trimmed() == "NOBIND") {
                    isNobindChecked = true;
                    ui->cbNobind->setChecked(true);
                } else {
                    ui->cbNobind->setChecked(false);
                }
            }
        if (!isPersistKeyChecked){
                if (line.toUpper().trimmed() == "PERSIST-KEY") {
                    isPersistKeyChecked = true;
                    ui->cbPersistKey->setChecked(true);
                } else {
                    ui->cbPersistKey->setChecked(false);
                }
            }
        if (!isPersistTunChecked){
                if (line.toUpper().trimmed() == "PERSIST-TUN") {
                    isPersistTunChecked = true;
                    ui->cbPersistTun->setChecked(true);
                } else {
                    ui->cbPersistTun->setChecked(false);
                }
            }
        if (!isCertChecked){
                if (line.left(4).toUpper().trimmed() == "CERT") {
                    isCertChecked = true;
                    QStringList keyvalList = line.split(" ");
                    if (keyvalList.size() != 2)
                        ui->txtCert->setText("");
                    else {
                        QString val = keyvalList[1];
                        ui->txtCert->setText(val.replace("\"",""));
                    }
                }
            }
        if (!isKeyChecked){
                if (line.left(3).toUpper().trimmed() == "KEY") {
                    isKeyChecked = true;
                    QStringList keyvalList = line.split(" ");
                    if (keyvalList.size() != 2)
                        ui->txtKey->setText("");
                    else {
                        QString val = keyvalList[1];
                        ui->txtKey->setText(val.replace("\"",""));
                    }
                }
            }
        if (!isCompLzoChecked){
                if (line.toUpper().trimmed() == "COMP-LZO") {
                    isCompLzoChecked = true;
                    ui->cbCompLzo->setChecked(true);
                } else {
                    ui->cbCompLzo->setChecked(false);
                }
            }
        if (!isVerboseChecked){
                if (line.left(4).toUpper().trimmed() == "VERB") {
                    isVerboseChecked = true;
                    QStringList keyvalList = line.split(" ");
                    if (keyvalList.size() != 2)
                        ui->txtVerbose->setText("");
                    else {
                        QString val = keyvalList[1];
                        ui->txtVerbose->setText(val.trimmed());
                    }
                }
            }
        if (!isMuteChecked){
                if (line.left(4).toUpper().trimmed() == "MUTE") {
                    isMuteChecked = true;
                    QStringList keyvalList = line.split(" ");
                    if (keyvalList.size() != 2)
                        ui->txtMute->setText("");
                    else {
                        QString val = keyvalList[1];
                        ui->txtMute->setText(val.trimmed());
                    }
                }
            }
        if (!isNoCacheChecked){
                if (line.toUpper().trimmed() == "AUTH-NOCACHE") {
                    isNoCacheChecked = true;
                    ui->cbNoCache->setChecked(true);
                } else {
                    ui->cbNoCache->setChecked(false);
                }
            }
        if (!isUserPassChecked){
                if (line.toUpper().trimmed() == "AUTH-USER-PASS") {
                    isUserPassChecked = true;
                    ui->cbUserPass->setChecked(true);
                } else {

                    ui->cbUserPass->setChecked(false);
                }
            }
        if (!isRouteMethodChecked){
                if (line.left(12).toUpper().trimmed() == "ROUTE-METHOD") {
                    isRouteMethodChecked = true;
                    QStringList keyvalList = line.split(" ");
                    if (keyvalList.size() != 2)
                        ui->cmbRouteMethod->setCurrentIndex(0);
                    else {
                        QString val = keyvalList[1];
                        if (val.trimmed().toUpper() == "EXE")
                            ui->cmbRouteMethod->setCurrentIndex(1);
                        else
                            ui->cmbRouteMethod->setCurrentIndex(2);
                    }
                }
            }
        if (!isRouteDelayChecked){
                if (line.left(11).toUpper().trimmed() == "ROUTE-DELAY") {
                    isRouteDelayChecked = true;
                    QStringList keyvalList = line.split(" ");
                    if (keyvalList.size() != 2)
                        ui->txtRouteDelay->setText("");
                    else {
                        QString val = keyvalList[1];
                        ui->txtRouteDelay->setText(val.trimmed());
                    }
                }
            }
        if (!isDevNodeChecked){
                if (line.left(8).toUpper().trimmed() == "DEV-NODE") {
                    isDevNodeChecked = true;
                    QStringList keyvalList = line.split(" ");
                    if (keyvalList.size() != 2)
                        ui->txtDevNode->setText("");
                    else {
                        QString val = keyvalList[1];
                        ui->txtDevNode->setText(val.trimmed());
                    }
                }
            }
        if (!isRandomHostChecked){
                if (line.toUpper().trimmed() == "REMOTE-RANDOM") {
                    isRandomHostChecked = true;
                    ui->cbRandomHost->setChecked(true);
                } else {
                    ui->cbRandomHost->setChecked(false);
                }
            }
        if (!isResolveHostChecked){
                if (line.left(13).toUpper().trimmed() == "RESOLVE-RETRY") {
                    isResolveHostChecked = true;
                    ui->cbResolveHostname->setChecked(true);
                } else {
                    ui->cbResolveHostname->setChecked(false);
                }
            }
        if (!isLinuxUserChecked){
                if (line.left(4).toUpper().trimmed() == "USER") {
                    isLinuxUserChecked = true;
                    QStringList keyvalList = line.split(" ");
                    if (keyvalList.size() != 2)
                        ui->txtLinuxUser->setText("");
                    else {
                        QString val = keyvalList[1];
                        ui->txtLinuxUser->setText(val.trimmed());
                    }
                }
            }
        if (!isLinuxGroupChecked){
                if (line.left(5).toUpper().trimmed() == "GROUP") {
                    isLinuxGroupChecked = true;
                    QStringList keyvalList = line.split(" ");
                    if (keyvalList.size() != 2)
                        ui->txtLinuxGroup->setText("");
                    else {
                        QString val = keyvalList[1];
                        ui->txtLinuxGroup->setText(val.trimmed());
                    }
                }
            }
        if (!isHttpRetryChecked){
            if (line.trimmed().left(16).contains("HTTP-PROXY-RETRY", Qt::CaseInsensitive)) {
                    isHttpRetryChecked = true;
                    ui->cbHTTPRetry->setChecked(true);
                } else {
                    ui->cbHTTPRetry->setChecked(false);
                }
            }

        if (!isHttpProxyChecked){
            if (line.trimmed().left(11).contains("HTTP-PROXY ", Qt::CaseInsensitive)) {
                    isHttpProxyChecked = true;
                    QStringList keyvalList = line.split(" ");
                    if (keyvalList.size() != 3){
                        ui->txtHTTPIP->setText("");
                        ui->txtHTTPPort->setText("");
                    } else {
                        QString val = keyvalList[1];
                        QString valport = keyvalList[2];
                        ui->txtHTTPIP->setText(val.replace("\"",""));
                        ui->txtHTTPPort->setText(valport.replace("\"",""));
                    }
                }
            }
        if (!isWirelessMuteWarningChecked){
                if (line.toUpper().trimmed() == "MUTE-REPLAY-WARNINGS") {
                    isWirelessMuteWarningChecked = true;
                    ui->cbMuteWirelessWarning->setChecked(true);
                } else {
                    ui->cbMuteWirelessWarning->setChecked(false);
                }
            }        
        if (!isCipherChecked){
                if (line.left(6).toUpper().trimmed() == "CIPHER") {
                    isCipherChecked = true;
                    QStringList keyvalList = line.split(" ");
                    if (keyvalList.size() != 2)
                        ui->cmbChiper->setCurrentIndex(0);
                    else {
                        QString val = keyvalList[1];
                        ui->cmbChiper->setCurrentIndex(this->getIndexFromCipher(val.trimmed()));
                    }
                }
            }
        if (!isCertTyoeServerChecked){
                if (line.toUpper().trimmed() == "NS-CERT-TYPE SERVER") {
                    isCertTyoeServerChecked = true;
                    ui->cbCertIsServer->setChecked(true);
                } else {
                    ui->cbCertIsServer->setChecked(false);
                }
            }

    }
    cf.close();
   // Scripts einlesen
   ui->txtAfterConnect->setText(this->configObj->getScript ("AC").replace("\"", ""));
   ui->txtAfterDisconnect->setText(this->configObj->getScript ("AD").replace("\"", ""));
   ui->txtBeforeConnect->setText(this->configObj->getScript ("BC").replace("\"", ""));
   ui->txtBeforeDisconnect->setText(this->configObj->getScript ("BD").replace("\"", ""));
   ui->txtErrorConnect->setText(this->configObj->getScript ("EC").replace("\"", ""));
   ui->txtScriptACDelay->setText((this->configObj->getScript ("TO") == "" ? "5000" : this->configObj->getScript ("TO")));
}

void ManageConnection::on_cmdSave_clicked()
{
    QStringList oldFields = this->getAllFieldWhichNotIntoTheInterface();
    // GUI auslesen und config speichern
    QFile cf (this->configObj->configPath + "/" + this->configObj->configName + ".ovpn");

    if (!cf.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Securepoint SSL VPN"));
        msgBox.setText(tr("Save Configuration"));
        msgBox.setWindowIcon(QIcon(":/images/logo.png"));
        msgBox.setInformativeText(tr("Unable to open file!"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
        msgBox.exec();
        return;
    }

    QTextStream out(&cf);
    // Datei offen, fertig zum schreiben
    out << "##############################################\n";
    out << "### \n";
    out << "### Configuration file created by Securepoint SSL VPN " + QDate::currentDate().toString() + " - " + QTime::currentTime().toString() +  "\n";
    out << "### Project website: http://sourceforge.net/projects/securepoint/ \n";
    out << "### Securepoint GmbH, Salzstrasse 1, Lueneburg, Germany; www.securepoint.de \n";
    out << "### \n";
    out << "### For further information about the configuration file, \n";
    out << "### please visit: http://www.openvpn.net/index.php/open-source/documentation\n";
    out << "### \n";
    out << "##############################################\n\n";
    if (ui->cbClient->isChecked())
        out << "client\n";
    if (ui->cbFloat->isChecked())
        out << "float\n";
    if (ui->cbCompLzo->isChecked())
        out << "comp-lzo\n";
    if (ui->cbFloat->isChecked())
        out << "float\n";
    if (ui->cbNobind->isChecked())
        out << "nobind\n";
    if (ui->cbPersistKey->isChecked())
        out << "persist-key\n";
    if (ui->cbPersistTun->isChecked())
        out << "persist-tun\n";
    if (ui->cbNoCache->isChecked())
        out << "auth-nocache\n";
    if (ui->cbUserPass->isChecked())
        out << "auth-user-pass\n";

    if (ui->cmbDev->currentIndex() != 0) {
        if (ui->cmbDev->currentIndex() == 1)
            out << "dev tun\n";
        else
            out << "dev tap\n";
    }

    out << "tun-mtu " << ui->txtMtu->text() << "\n";
    out << "remote " << ui->txtRemote->text() << " " + ui->txtRemotePort->text() << "\n";

    if (ui->cmbProto->currentIndex() != 0){
        if (ui->cmbProto->currentIndex() == 1)
            out << "proto tcp\n";
        else
            out << "proto udp\n";
    }

    if (ui->txtCA->text() != "")
        out << "ca \"" << ui->txtCA->text().replace("/", "\\\\") << "\"\n";
    if (ui->txtCert->text() != "")
        out << "cert \"" << ui->txtCert->text().replace("/", "\\\\") << "\"\n";
    if (ui->txtKey->text() != "")
        out << "key \"" << ui->txtKey->text().replace("/", "\\\\") << "\"\n";

    if (ui->cbCertIsServer->isChecked())
        out << "ns-cert-type server\n";

    if (ui->txtLinuxUser->text() != "")
        out << "user " << ui->txtLinuxUser->text() << "\n";
    if (ui->txtLinuxGroup->text() != "")
        out << "group " << ui->txtLinuxGroup->text() << "\n";

    if (ui->cbMsfix->isChecked())
        out << "mssfix\n";

    if (ui->txtFragment->text() != "")
        out << "fragment " << ui->txtFragment->text() << "\n";

    if (ui->cmbRouteMethod->currentIndex() != 0){
        if (ui->cmbRouteMethod->currentIndex() == 1)
            out << "route-method exe\n";
        else
            out << "route-method ipapi\n";
    }

    if (ui->txtDevNode->text() != "")
        out << "dev-node " << ui->txtDevNode->text() << "\n";

    if (ui->txtVerbose->text() != "")
        out << "verb " << ui->txtVerbose->text() << "\n";

    if (ui->txtRouteDelay->text() != "")
        out << "route-delay " << ui->txtRouteDelay->text() << "\n";

    if (ui->txtMute->text() != "")
        out << "mute " << ui->txtMute->text() << "\n";

    if (ui->cbRandomHost->isChecked())
        out << "remote-random\n";

    if (ui->cbResolveHostname->isChecked())
        out << "resolve-retry infinite\n";

    if (ui->cbHTTPRetry->isChecked())
        out << "http-proxy-retry\n";

    if (ui->txtHTTPIP->text() != "")
        out << "http-proxy " << ui->txtHTTPIP->text() << " " << ui->txtHTTPPort->text() << "\n";


    if (ui->cbMuteWirelessWarning->isChecked())
        out << "mute-replay-warnings\n";

    if (ui->cmbChiper->currentIndex() != 0)
        out << "cipher " << ui->cmbChiper->itemText(ui->cmbRouteMethod->currentIndex()) << "\n";

    if (ui->cbRedirectGateway->isChecked())
        out << "redirect-gateway\n";

    if (oldFields.size() > 0) {
       // Alte Felder die nicht in der GUI sind vorhanden
        out << "\n#Fields which could be not manage wih GUI \n\n";
        out << oldFields.join("\n");
    }

    // Datei schließen
    cf.close();
    // Scripts ablegen
    bool scriptDataWrite = false;
    QFileDialog certFileDialog;    
    QFile sf (this->configObj->configPath + QString("/scripts.conf"));

    if (!sf.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Securepoint SSL VPN"));
        msgBox.setText(tr("Save Scripts"));
        msgBox.setWindowIcon(QIcon(":/images/logo.png"));
        msgBox.setInformativeText(tr("Unable to open file!"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
        msgBox.exec();
        return;
    }

    QTextStream sout(&sf);
    if (ui->txtAfterConnect->text() != "") {
        sout << "AC:" << ui->txtAfterConnect->text() << "\n";
        if (ui->txtScriptACDelay->text() != "")
            sout << "TO:" << ui->txtScriptACDelay->text() << "\n";

        scriptDataWrite = true;
    }

    if (ui->txtAfterDisconnect->text() != "") {
        sout << "AD:" << ui->txtAfterDisconnect->text() << "\n";
        scriptDataWrite = true;
    }

    if (ui->txtBeforeConnect->text() != "") {
        sout << "BC:" << ui->txtBeforeConnect->text() << "\n";
        scriptDataWrite = true;
    }

    if (ui->txtBeforeDisconnect->text() != "") {
        sout << "BD:" << ui->txtBeforeDisconnect->text() << "\n";
        scriptDataWrite = true;
    }

    if (ui->txtErrorConnect->text() != "") {
        sout << "EC:" << ui->txtErrorConnect->text() << "\n";
        scriptDataWrite = true;
    }

    // Daten geschrieben?
    if (!scriptDataWrite) {
        if (!sf.remove()) {
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("Securepoint SSL VPN"));
            msgBox.setText(tr("Save Scripts"));
            msgBox.setWindowIcon(QIcon(":/images/logo.png"));
            msgBox.setInformativeText(tr("Can't delete scriptfile!"));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
            msgBox.exec();
            return;
        }
    }
    sf.close();
    /*
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Securepoint SSL VPN"));
    msgBox.setText(tr("Save configuration"));
    msgBox.setWindowIcon(QIcon(":/images/logo.png"));
    msgBox.setInformativeText(tr("Save successfully ended!"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
    msgBox.exec();*/
    this->close();
}

void ManageConnection::on_cmdGetCAPath_clicked()
{
    QFileDialog caFileDialog;
    QString filename = caFileDialog.getOpenFileName(this, tr("Find root ca"), this->lastDir, tr("Certificates (*.cert *.pem)"));
    if (filename != "") {
        int lastSlash = filename.lastIndexOf("/");
        QString filepath = filename.left(lastSlash);
        this->lastDir = filepath;
        if (filepath.replace("\\", "/").toLower() == this->configObj->configPath.replace("\\", "/").toLower())
            ui->txtCA->setText(filename.right(filename.size() - lastSlash -1));
        else
            ui->txtCA->setText(filename);
    }
}

void ManageConnection::on_cmdGetCertPath_clicked()
{
    QFileDialog certFileDialog;
    QString filename = certFileDialog.getOpenFileName(this, tr("Find certificates"), this->lastDir, tr("Certificates (*.cert *.pem)"));
    if (filename != "") {
        int lastSlash = filename.lastIndexOf("/");
        QString filepath = filename.left(lastSlash);
        this->lastDir = filepath;
        if (filepath.replace("\\", "/").toLower() == this->configObj->configPath.replace("\\", "/").toLower())
            ui->txtCert->setText(filename.right(filename.size() - lastSlash -1));
        else
            ui->txtCert->setText(filename);
    }
}

void ManageConnection::on_cmdGetKeyPath_clicked()
{
    QFileDialog keyFileDialog;
    QString filename = keyFileDialog.getOpenFileName(this, tr("Find key files"), this->lastDir, tr("Key files (*.key *.pem)"));
    if (filename != "") {
        int lastSlash = filename.lastIndexOf("/");
        QString filepath = filename.left(lastSlash);
        this->lastDir = filepath;
        if (filepath.replace("\\", "/").toLower() == this->configObj->configPath.replace("\\", "/").toLower())
            ui->txtKey->setText(filename.right(filename.size() - lastSlash -1));
        else
            ui->txtKey->setText(filename);
    }
}

void ManageConnection::on_cmdBeforeConnect_clicked()
{
    QFileDialog keyFileDialog;
    QString filename = keyFileDialog.getOpenFileName(this, tr("Find executeable files"), this->configObj->configPath, tr("All files (*.*)"));
    if (filename != "") {
            ui->txtBeforeConnect->setText(filename);
    }
}

void ManageConnection::on_cmdAfterConnect_clicked()
{
    QFileDialog keyFileDialog;
    QString filename = keyFileDialog.getOpenFileName(this, tr("Find executeable files"), this->configObj->configPath, tr("All files (*.*)"));
    if (filename != "") {
            ui->txtAfterConnect->setText(filename);
    }
}

void ManageConnection::on_cmdBeforeDisconnect_clicked()
{
    QFileDialog keyFileDialog;
    QString filename = keyFileDialog.getOpenFileName(this, tr("Find executeable files"), this->configObj->configPath, tr("All files (*.*)"));
    if (filename != "") {
            ui->txtBeforeDisconnect->setText(filename);
    }
}

void ManageConnection::on_cmdAfterDisconnect_clicked()
{
    QFileDialog keyFileDialog;
    QString filename = keyFileDialog.getOpenFileName(this, tr("Find executeable files"), this->configObj->configPath, tr("All files (*.*)"));
    if (filename != "") {
            ui->txtAfterDisconnect->setText(filename);
    }
}

void ManageConnection::on_cmdErrorConnect_clicked()
{
    QFileDialog keyFileDialog;
    QString filename = keyFileDialog.getOpenFileName(this, tr("Find executeable files"), this->configObj->configPath, tr("All files (*.*)"));
    if (filename != "") {
            ui->txtErrorConnect->setText(filename);
    }
}

QStringList ManageConnection::getAllFieldWhichNotIntoTheInterface() {
    // Load Config Data
    QStringList fieldsNotIncluded;
    QString cFile;
    fieldsNotIncluded.clear();
    cFile = this->configObj->configPath + QString("/") + this->configObj->configName + QString(".ovpn");
    QFile cf (cFile);

    if (!cf.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Securepoint SSL VPN"));
        msgBox.setText(tr("Load Configuration"));
        msgBox.setWindowIcon(QIcon(":/images/logo.png"));
        msgBox.setInformativeText(tr("Unable to read file!"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
        msgBox.exec();
        return fieldsNotIncluded;
    }


    QTextStream in(&cf);

    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.left(3).toUpper().trimmed() != "###"
                    && line.trimmed().replace("\n", "") != ""
                    && line.left(13).toUpper().trimmed() != "#FIELDS WHICH"
                    && line.toUpper().trimmed() != "CLIENT"
                    && line.left(2).toUpper().trimmed() != "CA"
                    && line.left(3).toUpper().trimmed() != "DEV"
                    && line.left(7).toUpper().trimmed() != "TUN-MTU"
                    && line.left(8).toUpper().trimmed() != "FRAGMENT"
                    && line.toUpper().trimmed() != "MSSFIX"
                    && line.toUpper().trimmed() != "REDIRECT-GATEWAY"
                    && line.left(5).toUpper().trimmed() != "PROTO"
                    && line.toUpper().trimmed() != "FLOAT"
                    && line.left(6).toUpper().trimmed() != "REMOTE"
                    && line.toUpper().trimmed() != "NOBIND"
                    && line.toUpper().trimmed() != "PERSIST-KEY"
                    && line.toUpper().trimmed() != "PERSIST-TUN"
                    && line.left(4).toUpper().trimmed() != "CERT"
                    && line.left(3).toUpper().trimmed() != "KEY"
                    && line.toUpper().trimmed() != "COMP-LZO"
                    && line.left(4).toUpper().trimmed() != "VERB"
                    && line.left(4).toUpper().trimmed() != "MUTE"
                    && line.toUpper().trimmed() != "AUTH-NOCACHE"
                    && line.toUpper().trimmed() != "AUTH-USER-PASS"
                    && line.left(12).toUpper().trimmed() != "ROUTE-METHOD"
                    && line.left(11).toUpper().trimmed() != "ROUTE-DELAY"
                    && line.left(8).toUpper().trimmed() != "DEV-NODE"
                    && line.toUpper().trimmed() != "REMOTE-RANDOM"
                    && line.left(13).toUpper().trimmed() != "RESOLVE-RETRY"
                    && line.left(4).toUpper().trimmed() != "USER"
                    && line.left(5).toUpper().trimmed() != "GROUP"
                    && !line.trimmed().left(16).contains("HTTP-PROXY-RETRY", Qt::CaseInsensitive)
                    && !line.trimmed().left(11).contains("HTTP-PROXY ", Qt::CaseInsensitive)
                    && line.toUpper().trimmed() != "MUTE-REPLAY-WARNINGS"
                    && line.left(6).toUpper().trimmed() != "CIPHER"
                    && line.toUpper().trimmed() != "NS-CERT-TYPE SERVER") {
                   fieldsNotIncluded.append(line);
                }
    }
    cf.close();
    return fieldsNotIncluded;
}
