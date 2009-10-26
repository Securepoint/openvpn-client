#include "preferences.h"
#include "ui_preferences.h"

Preferences::Preferences(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::Preferences)
{
    m_ui->setupUi(this);


    m_ui->lsvConfigs->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_ui->lsvConfigs, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(openContextMenuListView(const QPoint &)));
    connect(m_ui->lsvConfigs, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(openConfigFromListView(QListWidgetItem *)));

    // Init Widgets
    m_ui->cmbDev->insertItem(0, "Not defined");
    m_ui->cmbDev->insertItem(1, "tun");
    m_ui->cmbDev->insertItem(2, "tap");

    m_ui->cmbProto->insertItem(0, "Not defined");
    m_ui->cmbProto->insertItem(1, "TCP");
    m_ui->cmbProto->insertItem(2, "UDP");

    m_ui->cmbRouteMethod->insertItem(0, "Not defined");
    m_ui->cmbRouteMethod->insertItem(1, "Exe");
    m_ui->cmbRouteMethod->insertItem(2, "IPAPI");

    fillCipherCombo();
    // Set window title
    AppFunc app;
    if (app.isAppPortable())
        setWindowTitle("Securepoint OpenVPN Client Portable");
    else
        setWindowTitle("Securepoint OpenVPN Client");
}

Preferences::~Preferences()
{
    delete m_ui;
}

void Preferences::enableFields(bool flag) {
    // General
    m_ui->cbClient->setEnabled(flag);
    m_ui->cbFloat->setEnabled(flag);
    m_ui->cbCompLzo->setEnabled(flag);
    m_ui->cbNobind->setEnabled(flag);
    m_ui->cbPersistKey->setEnabled(flag);
    m_ui->cbPersistTun->setEnabled(flag);
    m_ui->cbNoCache->setEnabled(flag);
    m_ui->cbUserPass->setEnabled(flag);

    m_ui->cmbDev->setEnabled(flag);
    m_ui->txtMtu->setEnabled(flag);

    m_ui->txtRemote->setEnabled(flag);
    m_ui->txtRemotePort->setEnabled(flag);
    m_ui->cmbProto->setEnabled(flag);

    m_ui->txtCA->setEnabled(flag);
    m_ui->txtCert->setEnabled(flag);
    m_ui->txtKey->setEnabled(flag);
    m_ui->cbCertIsServer->setEnabled(flag);

    // Linux
    m_ui->txtLinuxUser->setEnabled(flag);
    m_ui->txtLinuxGroup->setEnabled(flag);

    // Windows

    m_ui->cbMsfix->setEnabled(flag);
    m_ui->txtFragment->setEnabled(flag);
    m_ui->cmbRouteMethod->setEnabled(flag);
    m_ui->txtDevNode->setEnabled(flag);

    // Advanced
    m_ui->txtVerbose->setEnabled(flag);
    m_ui->txtRouteDelay->setEnabled(flag);
    m_ui->txtMute->setEnabled(flag);

    m_ui->cbRandomHost->setEnabled(flag);
    m_ui->cbResolveHostname->setEnabled(flag);

    m_ui->cbHTTPRetry->setEnabled(flag);
    m_ui->txtHTTPIP->setEnabled(flag);
    m_ui->txtHTTPPort->setEnabled(flag);

    m_ui->cbMuteWirelessWarning->setEnabled(flag);

    m_ui->cmbChiper->setEnabled(flag);

    // Buttons
    m_ui->cmdSave->setEnabled(flag);
    m_ui->cmdGetCAPath->setEnabled(flag);
    m_ui->cmdGetCertPath->setEnabled(flag);
    m_ui->cmdGetKeyPath->setEnabled(flag);

    // Scripts
    m_ui->txtAfterConnect->setEnabled(flag);
    m_ui->txtAfterDisconnect->setEnabled(flag);
    m_ui->txtBeforeConnect->setEnabled(flag);
    m_ui->txtErrorConnect->setEnabled(flag);
    m_ui->txtBeforeDisconnect->setEnabled(flag);

    m_ui->cmdAfterConnect->setEnabled(flag);
    m_ui->cmdAfterDisconnect->setEnabled(flag);
    m_ui->cmdBeforeConnect->setEnabled(flag);
    m_ui->cmdBeforeDisconnect->setEnabled(flag);
    m_ui->cmdErrorConnect->setEnabled(flag);

    m_ui->txtScriptACDelay->setEnabled(flag);
}

void Preferences::resetFields() {
    m_ui->cbClient->setChecked(false);
    m_ui->cbFloat->setChecked(false);
    m_ui->cbCompLzo->setChecked(false);
    m_ui->cbNobind->setChecked(false);
    m_ui->cbPersistKey->setChecked(false);
    m_ui->cbPersistTun->setChecked(false);
    m_ui->cbNoCache->setChecked(false);
    m_ui->cbUserPass->setChecked(false);

    m_ui->cmbDev->setCurrentIndex(0);
    m_ui->txtMtu->setText("");

    m_ui->txtRemote->setText("");
    m_ui->txtRemotePort->setText("");
    m_ui->cmbProto->setCurrentIndex(0);

    m_ui->txtCA->setText("");
    m_ui->txtCert->setText("");
    m_ui->txtKey->setText("");
    m_ui->cbCertIsServer->setChecked(false);

    // Linux
    m_ui->txtLinuxUser->setText("");
    m_ui->txtLinuxGroup->setText("");

    // Windows

    m_ui->cbMsfix->setChecked(false);
    m_ui->txtFragment->setText("");
    m_ui->cmbRouteMethod->setCurrentIndex(0);
    m_ui->txtDevNode->setText("");

    // Advanced
    m_ui->txtVerbose->setText("");
    m_ui->txtRouteDelay->setText("");
    m_ui->txtMute->setText("");

    m_ui->cbRandomHost->setChecked(false);
    m_ui->cbResolveHostname->setChecked(false);

    m_ui->cbHTTPRetry->setChecked(false);
    m_ui->txtHTTPIP->setText("");
    m_ui->txtHTTPPort->setText("");

    m_ui->cbMuteWirelessWarning->setChecked(false);

    m_ui->cmbChiper->setCurrentIndex(0);

    m_ui->txtAfterConnect->setText("");
    m_ui->txtAfterDisconnect->setText("");
    m_ui->txtBeforeConnect->setText("");
    m_ui->txtBeforeDisconnect->setText("");
    m_ui->txtErrorConnect->setText("");

    m_ui->txtScriptACDelay->setText("5000");

}

void Preferences::openConfigFromListView (QListWidgetItem * item ){
    //m_ui->tbSettings->setCurrentIndex(0);
    enableFields(false);
    OpenVpnQListItem *t_Item = dynamic_cast<OpenVpnQListItem*>(item);

    // Load Config Data
    QString fileContent = QString("");
    QString cFile;
    this->actObject = t_Item->itemObject;
    cFile = t_Item->itemObject->configPath + QString("/") + t_Item->itemObject->configName + QString(".ovpn");
    m_ui->txtPathFromConfig->setText(cFile);
    QFile cf (cFile);

    if (!cf.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox msgBox;
                    msgBox.setText("Cannot read file.");
                    msgBox.exec();
         return;
    }

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

    while (!in.atEnd()) {
        QString line = in.readLine();
            if (!isClientChecked)
                if (line.toUpper().trimmed() == "CLIENT") {
                    isClientChecked = true;
                    m_ui->cbClient->setChecked(true);
                } else {
                    m_ui->cbClient->setChecked(false);
                }
            if (!isCAChecked)
                if (line.left(2).toUpper().trimmed() == "CA") {
                    isCAChecked = true;
                    QStringList keyvalList = line.split(" ");
                    if (keyvalList.size() != 2)
                        m_ui->txtCA->setText("");
                    else {
                        QString val = keyvalList[1];
                        m_ui->txtCA->setText(val.replace("\"",""));
                    }
                }
            if (!isDevChecked)
                if (line.left(3).toUpper().trimmed() == "DEV") {
                    isDevChecked = true;
                    QStringList keyvalList = line.split(" ");
                    if (keyvalList.size() != 2)
                        m_ui->cmbDev->setCurrentIndex(0);
                    else {
                        QString val = keyvalList[1];
                        if (val.trimmed().toUpper() == "TUN")
                            m_ui->cmbDev->setCurrentIndex(1);
                        else
                            m_ui->cmbDev->setCurrentIndex(2);
                    }
                }
            if (!isMtuChecked)
                if (line.left(7).toUpper().trimmed() == "TUN-MTU") {
                    isMtuChecked = true;
                    QStringList keyvalList = line.split(" ");
                    if (keyvalList.size() != 2)
                        m_ui->txtMtu->setText("");
                    else {
                        QString val = keyvalList[1];
                        m_ui->txtMtu->setText(val.trimmed());
                    }
                }
            if (!isFragmentChecked)
                if (line.left(8).toUpper().trimmed() == "FRAGMENT") {
                    isFragmentChecked = true;
                    QStringList keyvalList = line.split(" ");
                    if (keyvalList.size() != 2)
                        m_ui->txtFragment->setText("");
                    else {
                        QString val = keyvalList[1];
                        m_ui->txtFragment->setText(val.trimmed());
                    }
                }
            if (!isMsFixChecked)
                if (line.toUpper().trimmed() == "MSSFIX") {
                    isMsFixChecked = true;
                    m_ui->cbMsfix->setChecked(true);
                } else {
                    m_ui->cbMsfix->setChecked(false);
                }
            if (!isProtoChecked)
                if (line.left(5).toUpper().trimmed() == "PROTO") {
                    isProtoChecked = true;
                    QStringList keyvalList = line.split(" ");
                    if (keyvalList.size() != 2)
                        m_ui->cmbProto->setCurrentIndex(0);
                    else {
                        QString val = keyvalList[1];
                        if (val.trimmed().toUpper() == "TCP")
                            m_ui->cmbProto->setCurrentIndex(1);
                        else
                            m_ui->cmbProto->setCurrentIndex(2);
                    }
                }
            if (!isFloatChecked)
                if (line.toUpper().trimmed() == "FLOAT") {
                    isFloatChecked = true;
                    m_ui->cbFloat->setChecked(true);
                } else {
                    m_ui->cbFloat->setChecked(false);
                }
            if (!isRemoteChecked)
                if (line.left(6).toUpper().trimmed() == "REMOTE") {
                    isRemoteChecked = true;
                    QStringList keyvalList = line.split(" ");
                    if (keyvalList.size() != 3){
                        m_ui->txtRemotePort->setText("");
                        m_ui->txtRemote->setText("");
                    } else {
                        QString val = keyvalList[1];
                        QString valport = keyvalList[2];
                        m_ui->txtRemote->setText(val.replace("\"",""));
                        m_ui->txtRemotePort->setText(valport.replace("\"",""));
                    }
                }
            if (!isNobindChecked)
                if (line.toUpper().trimmed() == "NOBIND") {
                    isNobindChecked = true;
                    m_ui->cbNobind->setChecked(true);
                } else {
                    m_ui->cbNobind->setChecked(false);
                }
            if (!isPersistKeyChecked)
                if (line.toUpper().trimmed() == "PERSIST-KEY") {
                    isPersistKeyChecked = true;
                    m_ui->cbPersistKey->setChecked(true);
                } else {
                    m_ui->cbPersistKey->setChecked(false);
                }
            if (!isPersistTunChecked)
                if (line.toUpper().trimmed() == "PERSIST-TUN") {
                    isPersistTunChecked = true;
                    m_ui->cbPersistTun->setChecked(true);
                } else {
                    m_ui->cbPersistTun->setChecked(false);
                }
            if (!isCertChecked)
                if (line.left(4).toUpper().trimmed() == "CERT") {
                    isCertChecked = true;
                    QStringList keyvalList = line.split(" ");
                    if (keyvalList.size() != 2)
                        m_ui->txtCert->setText("");
                    else {
                        QString val = keyvalList[1];
                        m_ui->txtCert->setText(val.replace("\"",""));
                    }
                }
             if (!isKeyChecked)
                if (line.left(3).toUpper().trimmed() == "KEY") {
                    isKeyChecked = true;
                    QStringList keyvalList = line.split(" ");
                    if (keyvalList.size() != 2)
                        m_ui->txtKey->setText("");
                    else {
                        QString val = keyvalList[1];
                        m_ui->txtKey->setText(val.replace("\"",""));
                    }
                }
             if (!isCompLzoChecked)
                if (line.toUpper().trimmed() == "COMP-LZO") {
                    isCompLzoChecked = true;
                    m_ui->cbCompLzo->setChecked(true);
                } else {
                    m_ui->cbCompLzo->setChecked(false);
                }
             if (!isVerboseChecked)
                if (line.left(4).toUpper().trimmed() == "VERB") {
                    isVerboseChecked = true;
                    QStringList keyvalList = line.split(" ");
                    if (keyvalList.size() != 2)
                        m_ui->txtVerbose->setText("");
                    else {
                        QString val = keyvalList[1];
                        m_ui->txtVerbose->setText(val.trimmed());
                    }
                }
             if (!isMuteChecked)
                if (line.left(4).toUpper().trimmed() == "MUTE") {
                    isMuteChecked = true;
                    QStringList keyvalList = line.split(" ");
                    if (keyvalList.size() != 2)
                        m_ui->txtMute->setText("");
                    else {
                        QString val = keyvalList[1];
                        m_ui->txtMute->setText(val.trimmed());
                    }
                }
             if (!isNoCacheChecked)
                if (line.toUpper().trimmed() == "AUTH-NOCACHE") {
                    isNoCacheChecked = true;
                    m_ui->cbNoCache->setChecked(true);
                } else {
                    m_ui->cbNoCache->setChecked(false);
                }
             if (!isUserPassChecked)
                if (line.toUpper().trimmed() == "AUTH-USER-PASS") {
                    isUserPassChecked = true;
                    m_ui->cbUserPass->setChecked(true);
                } else {

                    m_ui->cbUserPass->setChecked(false);
                }
             if (!isRouteMethodChecked)
                if (line.left(12).toUpper().trimmed() == "ROUTE-METHOD") {
                    isRouteMethodChecked = true;
                    QStringList keyvalList = line.split(" ");
                    if (keyvalList.size() != 2)
                        m_ui->cmbRouteMethod->setCurrentIndex(0);
                    else {
                        QString val = keyvalList[1];
                        if (val.trimmed().toUpper() == "EXE")
                            m_ui->cmbRouteMethod->setCurrentIndex(1);
                        else
                            m_ui->cmbRouteMethod->setCurrentIndex(2);
                    }
                }
             if (!isRouteDelayChecked)
                if (line.left(11).toUpper().trimmed() == "ROUTE-DELAY") {
                    isRouteDelayChecked = true;
                    QStringList keyvalList = line.split(" ");
                    if (keyvalList.size() != 2)
                        m_ui->txtRouteDelay->setText("");
                    else {
                        QString val = keyvalList[1];
                        m_ui->txtRouteDelay->setText(val.trimmed());
                    }
                }
             if (!isDevNodeChecked)
                if (line.left(8).toUpper().trimmed() == "DEV-NODE") {
                    isDevNodeChecked = true;
                    QStringList keyvalList = line.split(" ");
                    if (keyvalList.size() != 2)
                        m_ui->txtDevNode->setText("");
                    else {
                        QString val = keyvalList[1];
                        m_ui->txtDevNode->setText(val.trimmed());
                    }
                }
             if (!isRandomHostChecked)
                if (line.toUpper().trimmed() == "REMOTE-RANDOM") {
                    isRandomHostChecked = true;
                    m_ui->cbRandomHost->setChecked(true);
                } else {
                    m_ui->cbRandomHost->setChecked(false);
                }
             if (!isResolveHostChecked)
                if (line.left(12).toUpper().trimmed() == "RESOLVE-RETRY") {
                    isResolveHostChecked = true;
                    m_ui->cbResolveHostname->setChecked(true);
                } else {
                    m_ui->cbResolveHostname->setChecked(false);
                }
            if (!isLinuxUserChecked)
                if (line.left(4).toUpper().trimmed() == "USER") {
                    isLinuxUserChecked = true;
                    QStringList keyvalList = line.split(" ");
                    if (keyvalList.size() != 2)
                        m_ui->txtLinuxUser->setText("");
                    else {
                        QString val = keyvalList[1];
                        m_ui->txtLinuxUser->setText(val.trimmed());
                    }
                }
            if (!isLinuxGroupChecked)
                if (line.left(4).toUpper().trimmed() == "USER") {
                    isLinuxGroupChecked = true;
                    QStringList keyvalList = line.split(" ");
                    if (keyvalList.size() != 2)
                        m_ui->txtLinuxGroup->setText("");
                    else {
                        QString val = keyvalList[1];
                        m_ui->txtLinuxGroup->setText(val.trimmed());
                    }
                }
           if (!isHttpRetryChecked)
                if (line.toUpper().trimmed() == "HTTP-PROXY-RETRY") {
                    isHttpRetryChecked = true;
                    m_ui->cbHTTPRetry->setChecked(true);
                } else {
                    m_ui->cbHTTPRetry->setChecked(false);
                }
           if (!isHttpProxyChecked)
                if (line.left(11).toUpper().trimmed() == "HTTP-PROXY ") {
                    isHttpProxyChecked = true;
                    QStringList keyvalList = line.split(" ");
                    if (keyvalList.size() != 3){
                        m_ui->txtHTTPIP->setText("");
                        m_ui->txtHTTPPort->setText("");
                    } else {
                        QString val = keyvalList[1];
                        QString valport = keyvalList[2];
                        m_ui->txtHTTPIP->setText(val.replace("\"",""));
                        m_ui->txtHTTPPort->setText(valport.replace("\"",""));
                    }
                }
          if (!isWirelessMuteWarningChecked)
                if (line.toUpper().trimmed() == "MUTE-REPLAY-WARNINGS") {
                    isWirelessMuteWarningChecked = true;
                    m_ui->cbMuteWirelessWarning->setChecked(true);
                } else {
                    m_ui->cbMuteWirelessWarning->setChecked(false);
                }
          if (!isCipherChecked)
                if (line.left(5).toUpper().trimmed() == "CIPHER") {
                    isCipherChecked = true;
                    QStringList keyvalList = line.split(" ");
                    if (keyvalList.size() != 2)
                        m_ui->cmbChiper->setCurrentIndex(0);
                    else {
                        QString val = keyvalList[1];
                        m_ui->cmbChiper->setCurrentIndex(getIndexFromCipher(val.trimmed()));
                    }
                }
          if (!isCertTyoeServerChecked)
                if (line.toUpper().trimmed() == "NS-CERT-TYPE SERVER") {
                    isCertTyoeServerChecked = true;
                    m_ui->cbCertIsServer->setChecked(true);
                } else {
                    m_ui->cbCertIsServer->setChecked(false);
                }


    }
    cf.close();
   // Scripts einlesen
   m_ui->txtAfterConnect->setText(actObject->getScript ("AC"));
   m_ui->txtAfterDisconnect->setText(actObject->getScript ("AD"));
   m_ui->txtBeforeConnect->setText(actObject->getScript ("BC"));
   m_ui->txtBeforeDisconnect->setText(actObject->getScript ("BD"));
   m_ui->txtErrorConnect->setText(actObject->getScript ("EC"));
   m_ui->txtScriptACDelay->setText((actObject->getScript ("TO") == "" ? "5000" : actObject->getScript ("TO")));
}

void Preferences::fillCipherCombo() {
    m_ui->cmbChiper->insertItem(0, "Standard");
    m_ui->cmbChiper->insertItem(1, "DES-CFB");
    m_ui->cmbChiper->insertItem(2, "DES-CBC");
    m_ui->cmbChiper->insertItem(3, "RC2-CBC");
    m_ui->cmbChiper->insertItem(4, "RC2-CFB");
    m_ui->cmbChiper->insertItem(5, "RC2-OFB");
    m_ui->cmbChiper->insertItem(6, "DES-EDE-CBC");
    m_ui->cmbChiper->insertItem(7, "DES-EDE3-CBC");
    m_ui->cmbChiper->insertItem(8, "DES-OFB");
    m_ui->cmbChiper->insertItem(9, "DES-EDE-CFB");
    m_ui->cmbChiper->insertItem(10, "DES-EDE3-CFB");
    m_ui->cmbChiper->insertItem(11, "DES-EDE-OFB");
    m_ui->cmbChiper->insertItem(12, "DES-EDE3-OFB");
    m_ui->cmbChiper->insertItem(13, "DESX-CBC");
    m_ui->cmbChiper->insertItem(14, "BF-CBC");
    m_ui->cmbChiper->insertItem(15, "BF-CFB");
    m_ui->cmbChiper->insertItem(16, "BF-OFB");
    m_ui->cmbChiper->insertItem(17, "RC2-40-CBC");
    m_ui->cmbChiper->insertItem(18, "CAST5-CBC");
    m_ui->cmbChiper->insertItem(19, "CAST5-CFB");
    m_ui->cmbChiper->insertItem(20, "CAST5-OFB");
    m_ui->cmbChiper->insertItem(21, "RC2-64-CBC");
    m_ui->cmbChiper->insertItem(22, "AES-128-CBC");
    m_ui->cmbChiper->insertItem(23, "AES-128-OFB");
    m_ui->cmbChiper->insertItem(24, "AES-128-CFB");
    m_ui->cmbChiper->insertItem(25, "AES-192-CBC");
    m_ui->cmbChiper->insertItem(26, "AES-192-OFB");
    m_ui->cmbChiper->insertItem(27, "AES-192-CFB");
    m_ui->cmbChiper->insertItem(28, "AES-256-CBC");
    m_ui->cmbChiper->insertItem(29, "AES-256-OFB");
    m_ui->cmbChiper->insertItem(30, "AES-256-CFB");
    m_ui->cmbChiper->insertItem(31, "AES-128-CFB1");
    m_ui->cmbChiper->insertItem(32, "AES-192-CFB1");
    m_ui->cmbChiper->insertItem(33, "AES-256-CFB1");
    m_ui->cmbChiper->insertItem(34, "AES-128-CFB8");
    m_ui->cmbChiper->insertItem(35, "AES-192-CFB8");
    m_ui->cmbChiper->insertItem(36, "AES-256-CFB8");
    m_ui->cmbChiper->insertItem(37, "DES-CFB1");
    m_ui->cmbChiper->insertItem(38, "DES-CFB8");
}

int Preferences::getIndexFromCipher(QString cipher) {
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

void Preferences::openContextMenuListView(const QPoint &pos) {
    if (this->actObject->configName != "") {
        QMenu *configPopUp = new QMenu (this);
        configPopUp->addAction(QPixmap(":/images/edit.svg"), "&Edit", this, SLOT(editConfig()));
        //configPopUp->addSeparator();
        //configPopUp->addAction(QPixmap(":/images/delete.svg"), "&Delete", this, SLOT(deleteConfig()));
        configPopUp->exec(m_ui->lsvConfigs->mapToGlobal(pos));
    }
}

void Preferences::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Preferences::on_cmdClose_clicked()
{
    this->close();
}

void Preferences::editConfig() {
        enableFields(true);
}

void Preferences::deleteConfig() {
    /*
    enableFields(false);
    // Config, CA, Cert, Key löschen
    if (m_ui->txtCA->text() != "") {
        QString caPath;
        if (m_ui->txtCA->text().indexOf("/") != -1)
            caPath = m_ui->txtCA->text();
        else
            caPath = this->actObject->configPath + "/" + m_ui->txtCA->text();

        QFile caFile(caPath);

        if (!caFile.remove()) {
            QMessageBox::critical(0, QString("OpenVPN Client"), caFile.errorString());
        }
    }

    if (m_ui->txtCert->text() != "") {
        QString certPath;
        if (m_ui->txtCert->text().indexOf("/") != -1)
            certPath = m_ui->txtCert->text();
        else
            certPath = this->actObject->configPath + "/" + m_ui->txtCert->text();

        QFile certFile(certPath);

        if (!certFile.remove()) {
            QMessageBox::critical(0, QString("OpenVPN Client"), certFile.errorString());
        }
    }
    if (m_ui->txtKey->text() != "") {
        QString keyPath;
        if (m_ui->txtKey->text().indexOf("/") != -1)
            keyPath = m_ui->txtKey->text();
        else
            keyPath = this->actObject->configPath + "/" + m_ui->txtKey->text();

        QFile keyFile(keyPath);

        if (!keyFile.remove()) {
            QMessageBox::critical(0, QString("OpenVPN Client"), keyFile.errorString());
        }

    }

    QFile configFile(this->actObject->configPath + "/" + this->actObject->configName + ".ovpn");
    if (!configFile.remove()) {
        QMessageBox::critical(0, QString("OpenVPN Client"), configFile.errorString());
    }
    delete this->actObject;
*/
}

void Preferences::openDialog(QList<OpenVpn*> configList) {
    resetFields();
    enableFields(false);
    m_ui->tbSettings->setCurrentIndex(0);
    // Zeiger auf Obj merken
    m_ui->lsvConfigs->clear();
    foreach (OpenVpn *configObj, configList) {
        OpenVpnQListItem *newItem = new OpenVpnQListItem (configObj);
        newItem->setText(configObj->configName + (configObj->isConnectionStable() ? "  [connected]" : ""));
        m_ui->lsvConfigs->insertItem(1, newItem);
    }
    m_ui->memHelp->setText("Please click the info symbol of the item you want to see the help.\nFor further informations please visit: http://www.openvpn.net");
    this->show();
}

void Preferences::on_cmdAddConfig_clicked()
{
    vpnwiz.show();
}

void Preferences::on_cmdSave_clicked()
{
    // GUI auslesen und config speichern
    QFile cf (m_ui->txtPathFromConfig->text());

    if (!cf.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QMessageBox msgBox;
                    msgBox.setText("Cannot open file to write.");
                    msgBox.exec();
         return;
    }

    QTextStream out(&cf);
    // Datei offen, fertig zum schreiben
    if (m_ui->cbClient->isChecked())
        out << "client\n";
    if (m_ui->cbFloat->isChecked())
        out << "float\n";
    if (m_ui->cbCompLzo->isChecked())
        out << "comp-lzo\n";
    if (m_ui->cbFloat->isChecked())
        out << "float\n";
    if (m_ui->cbNobind->isChecked())
        out << "nobind\n";
    if (m_ui->cbPersistKey->isChecked())
        out << "persist-key\n";
    if (m_ui->cbPersistTun->isChecked())
        out << "persist-tun\n";
    if (m_ui->cbNoCache->isChecked())
        out << "auth-nocache\n";
    if (m_ui->cbUserPass->isChecked())
        out << "auth-user-pass\n";

    if (m_ui->cmbDev->currentIndex() != 0)
        if (m_ui->cmbDev->currentIndex() == 1)
            out << "dev tun\n";
        else
            out << "dev tap\n";

    out << "tun-mtu " << m_ui->txtMtu->text() << "\n";
    out << "remote " << m_ui->txtRemote->text() << " " + m_ui->txtRemotePort->text() << "\n";

    if (m_ui->cmbProto->currentIndex() != 0)
        if (m_ui->cmbProto->currentIndex() == 1)
            out << "proto tcp\n";
        else
            out << "proto udp\n";

    if (m_ui->txtCA->text() != "")
        out << "ca \"" << m_ui->txtCA->text() << "\"\n";
    if (m_ui->txtCert->text() != "")
        out << "cert \"" << m_ui->txtCert->text() << "\"\n";
    if (m_ui->txtKey->text() != "")
        out << "key \"" << m_ui->txtKey->text() << "\"\n";

    if (m_ui->cbCertIsServer->isChecked())
        out << "ns-cert-type server\n";

    if (m_ui->txtLinuxUser->text() != "")
        out << "cert " << m_ui->txtLinuxUser->text() << "\n";
    if (m_ui->txtLinuxGroup->text() != "")
        out << "key " << m_ui->txtLinuxGroup->text() << "\n";

    if (m_ui->cbMsfix->isChecked())
        out << "mssfix\n";

    if (m_ui->txtFragment->text() != "")
        out << "fragment " << m_ui->txtFragment->text() << "\n";

    if (m_ui->cmbRouteMethod->currentIndex() != 0)
        if (m_ui->cmbRouteMethod->currentIndex() == 1)
            out << "route-method exe\n";
        else
            out << "route-method ipapi\n";

    if (m_ui->txtDevNode->text() != "")
        out << "dev-node " << m_ui->txtDevNode->text() << "\n";

    if (m_ui->txtVerbose->text() != "")
        out << "verb " << m_ui->txtVerbose->text() << "\n";

    if (m_ui->txtRouteDelay->text() != "")
        out << "route-delay " << m_ui->txtRouteDelay->text() << "\n";

    if (m_ui->txtMute->text() != "")
        out << "mute " << m_ui->txtMute->text() << "\n";

    if (m_ui->cbRandomHost->isChecked())
        out << "remote-random\n";

    if (m_ui->cbResolveHostname->isChecked())
        out << "resolv-retry infinite\n";

    if (m_ui->cbHTTPRetry->isChecked())
        out << "http-proxy-retry\n";

    if (m_ui->txtHTTPIP->text() != "")
        out << "http-proxy " << m_ui->txtHTTPIP->text() << " " << m_ui->txtHTTPPort->text() << "\n";


    if (m_ui->cbMuteWirelessWarning->isChecked())
        out << "mute-replay-warnings\n";

    if (m_ui->cmbChiper->currentIndex() != 0)
        out << "cipher " << m_ui->cmbChiper->itemText(m_ui->cmbRouteMethod->currentIndex()) << "\n";

    // Datei schließen
    cf.close();
    // Scripts ablegen
    bool scriptDataWrite = false;
    QFileDialog certFileDialog;
    QString filename = m_ui->txtPathFromConfig->text();
    if (filename != "") {
        int lastSlash = filename.lastIndexOf("/");
        QString filepath = filename.left(lastSlash) + QString("/scripts.conf");
        QFile sf (filepath);

        if (!sf.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
            QMessageBox msgBox;
                        msgBox.setText("Cannot open file to write.");
                        msgBox.exec();
             return;
        }

        QTextStream sout(&sf);
        if (m_ui->txtAfterConnect->text() != "") {
            sout << "AC:" << m_ui->txtAfterConnect->text() << "\n";
            if (m_ui->txtScriptACDelay->text() != "")
                sout << "TO:" << m_ui->txtScriptACDelay->text() << "\n";

            scriptDataWrite = true;
        }

        if (m_ui->txtAfterDisconnect->text() != "") {
            sout << "AD:" << m_ui->txtAfterDisconnect->text() << "\n";
            scriptDataWrite = true;
        }

        if (m_ui->txtBeforeConnect->text() != "") {
            sout << "BC:" << m_ui->txtBeforeConnect->text() << "\n";
            scriptDataWrite = true;
        }

        if (m_ui->txtBeforeDisconnect->text() != "") {
            sout << "BD:" << m_ui->txtBeforeDisconnect->text() << "\n";
            scriptDataWrite = true;
        }

        if (m_ui->txtErrorConnect->text() != "") {
            sout << "EC:" << m_ui->txtErrorConnect->text() << "\n";
            scriptDataWrite = true;
        }

        // Daten geschrieben?
        if (!scriptDataWrite) {
            if (!sf.remove()) {
                QMessageBox::critical(0,"Securepoint OpenVPN Client", "Can't delete scriptfile!");
                return;
            }
        }


        sf.close();
   }
}

void Preferences::on_cmdGetCAPath_clicked()
{
    QFileDialog caFileDialog;
    QString filename = caFileDialog.getOpenFileName(this, tr("Find root ca"), this->actObject->configPath, "Certificates (*.cert)");
    if (filename != "") {
        int lastSlash = filename.lastIndexOf("/");
        QString filepath = filename.left(lastSlash);

        if (filepath == this->actObject->configPath)
            m_ui->txtCA->setText(filename.right(filename.size() - lastSlash -1));
        else
            m_ui->txtCA->setText(filename);
    }
}

void Preferences::on_cmdGetCertPath_clicked()
{
    QFileDialog certFileDialog;
    QString filename = certFileDialog.getOpenFileName(this, tr("Find certificates"), this->actObject->configPath, "Certificates (*.cert)");
    if (filename != "") {
        int lastSlash = filename.lastIndexOf("/");
        QString filepath = filename.left(lastSlash);

        if (filepath == this->actObject->configPath)
            m_ui->txtCert->setText(filename.right(filename.size() - lastSlash -1));
        else
            m_ui->txtCert->setText(filename);
    }
}

void Preferences::on_cmdGetKeyPath_clicked()
{
    QFileDialog keyFileDialog;
    QString filename = keyFileDialog.getOpenFileName(this, tr("Find key files"), this->actObject->configPath, "Key files (*.key)");
    if (filename != "") {
        int lastSlash = filename.lastIndexOf("/");
        QString filepath = filename.left(lastSlash);

        if (filepath == this->actObject->configPath)
            m_ui->txtKey->setText(filename.right(filename.size() - lastSlash -1));
        else
            m_ui->txtKey->setText(filename);
    }
}

void Preferences::on_label_linkHovered(QString link)
{

}

void Preferences::on_cmdInfoClient_clicked()
{
   m_ui->memHelp->setText("Specify that this is a client. \nIt will be pulling certain config file directives from the server.");
}

void Preferences::on_cmdInfoRemote_clicked()
{
    m_ui->memHelp->setText("Specify the listen port from the OpenVPN server.");
}

void Preferences::on_cmdInfoRemoteIP_clicked()
{
    m_ui->memHelp->setText("Specify the IP from the OpenVPN server you want to connect.");
}

void Preferences::on_cmdInfoLinuxUser_clicked()
{
    m_ui->memHelp->setText("Specify the user name for the OpenVPN process which used after initialization.\nThe group field is similar to the user field.");
}

void Preferences::on_cmdInfoWinRouteMethod_clicked()
{
    m_ui->memHelp->setText("Specify which method to use for adding routes on Windows?\nipapi - Use IP helper API.\nexe - Use the route.exe .");
}

void Preferences::on_cmdInfoWinDevNode_clicked()
{
    m_ui->memHelp->setText("Specify the TAP-Win32 adapter name.\nIt is also possible to set the GUID of the adapter enclosed by braces.");
}

void Preferences::on_cmdInfoAdvRandomHost_clicked()
{
    m_ui->memHelp->setText("Specify that a random host is taken for the connection.\nThe host can be defined in the remote list.");
}

void Preferences::on_cmdInfoMuteWirelessWarnings_clicked()
{
    m_ui->memHelp->setText("Specify that the output of replay warnings will be silenced.\nReplay warnings are false alarms on Wireless networks.");
}

void Preferences::on_cmdInfoServerCertificate_clicked()
{
    m_ui->memHelp->setText("Specify that a certificate is used, which is created with the server attribute.");
}

void Preferences::on_cmdInfoDevName_clicked()
{
   m_ui->memHelp->setText("Specify the virtual interfaces for the connection.\nTap is normaly used on Microsoft Windows.\nTun is normaly used on Linux systems.");
}

void Preferences::on_cmdInfoAuthUserPass_clicked()
{
    m_ui->memHelp->setText("Specify that the user/pass authentication method is used.\nIt is required to type a username and a password for the connection.");
}

void Preferences::on_cmdInfoScriptsAfterConnect_clicked()
{
    m_ui->memHelp->setText("Specifiy the script which will be execute after a successfull connection to the OpenVPN network.");
}

void Preferences::on_cmdInfoScriptsBeforeConnect_clicked()
{
    m_ui->memHelp->setText("Specifiy the script which will be execute before the OpenVPN network will be connected.");
}

void Preferences::on_cmdInfoScriptsBeforeDisconnect_clicked()
{
    m_ui->memHelp->setText("Specifiy the script which will be execute before the OpenVPN network will be disconnected.");
}

void Preferences::on_cmdInfoScriptsAfterDisconnect_clicked()
{
    m_ui->memHelp->setText("Specifiy the script which will be execute after a successfull disconnect from the OpenVPN network.");
}

void Preferences::on_cmdInfoScriptsErrorConnect_clicked()
{
    m_ui->memHelp->setText("Specifiy the script which will be execute on an error while connecting to the OpenVPN network.");
}

void Preferences::on_cmdBeforeConnect_clicked()
{
    QFileDialog keyFileDialog;
    QString filename = keyFileDialog.getOpenFileName(this, tr("Find executeable files"), this->actObject->configPath, "All files (*.*)");
    if (filename != "") {
            m_ui->txtBeforeConnect->setText(filename);
    }
}

void Preferences::on_cmdAfterConnect_clicked()
{
    QFileDialog keyFileDialog;
    QString filename = keyFileDialog.getOpenFileName(this, tr("Find executeable files"), this->actObject->configPath, "All files (*.*)");
    if (filename != "") {
            m_ui->txtAfterConnect->setText(filename);
    }
}

void Preferences::on_cmdBeforeDisconnect_clicked()
{
    QFileDialog keyFileDialog;
    QString filename = keyFileDialog.getOpenFileName(this, tr("Find executeable files"), this->actObject->configPath, "All files (*.*)");
    if (filename != "") {
            m_ui->txtBeforeDisconnect->setText(filename);
    }
}

void Preferences::on_cmdAfterDisconnect_clicked()
{
    QFileDialog keyFileDialog;
    QString filename = keyFileDialog.getOpenFileName(this, tr("Find executeable files"), this->actObject->configPath, "All files (*.*)");
    if (filename != "") {
            m_ui->txtAfterDisconnect->setText(filename);
    }
}

void Preferences::on_cmdErrorConnect_clicked()
{
    QFileDialog keyFileDialog;
    QString filename = keyFileDialog.getOpenFileName(this, tr("Find executeable files"), this->actObject->configPath, "All files (*.*)");
    if (filename != "") {
            m_ui->txtErrorConnect->setText(filename);
    }
}

void Preferences::on_cmdInfoScriptsDelyAC_clicked()
{
    m_ui->memHelp->setText("Specifiy the delay to executing the after connection script after the OpenVPN connection established.\nThis is useful to prevent network errors.\nDefault is 5000ms");
}
