#include "AdvConfigSettingsWidget.h"
#include "ui_AdvConfigSettingsWidget.h"
#include <debug/debug.h>
#include <dialogs/inlineWindow.h>

#include <message.h>
#include <config/configvalues.h>
#include <Utils.h>
#include <QtGui>
#include <QtGui/qguiapplication.h>
#include <QScreen>
#include <QtWidgets/QFileDialog>

#include <checksum.h>

float windowsDpiScale();

AdvConfigSettingsWidget::AdvConfigSettingsWidget(ConnectionData *obj, QWidget * parent) :
    FramelessDialog(parent),
    ui(new Ui::AdvConfigSettingsWidget),
    configObj (obj)
{
    this->setWindowFlags(Qt::WindowCloseButtonHint);

    ui->setupUi((QDialog*)this->getWrapperWidget());

    QMetaObject::connectSlotsByName(this);

    auto geom = this->getWrapperWidget()->geometry();
    auto size = geom.size() * windowsDpiScale();

    int yScreen = (GetSystemMetrics(SM_CYSCREEN));
    if(size.height() > yScreen)
        size.setHeight(yScreen);
    geom.setSize(size);
    this->getWrapperWidget()->setGeometry(geom);

    this->setupFrameless();

    ui->groupBox_3->setLayout(ui->gridLayoutGeneral);
    this->ui->lvRemote->setModel(new QStandardItemModel(0, 2));

    this->ui->lvRemote->model()->setHeaderData( 0, Qt::Horizontal, "IP");
    this->ui->lvRemote->model()->setHeaderData( 1, Qt::Horizontal, "Port");

    this->ui->lvRemote->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);



}

void AdvConfigSettingsWidget::changeEvent(QEvent *e)
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

void AdvConfigSettingsWidget::showEvent(QShowEvent *e) {
    // Fenster init
    this->setWindowTitle(QObject::tr("Edit Connection - ") + this->configObj->GetConfigPath());
    // Init Widgets
    ui->cmbChiper->clear();
    ui->cmbDev->clear();
    ui->cmbProto->clear();
    ui->cmbRouteMethod->clear();

    ui->cmbDev->insertItem(0, QObject::tr("Not defined"));
    ui->cmbDev->insertItem(1, QLatin1String("tun"));
    ui->cmbDev->insertItem(2, QLatin1String("tap"));

    ui->cmbProto->insertItem(0, QObject::tr("Not defined"));
    ui->cmbProto->insertItem(1, QLatin1String("TCP"));
    ui->cmbProto->insertItem(2, QLatin1String("UDP"));

    ui->cmbRouteMethod->insertItem(0, QObject::tr("Not defined"));
    ui->cmbRouteMethod->insertItem(1, QLatin1String("Exe"));
    ui->cmbRouteMethod->insertItem(2, QLatin1String("IPAPI"));

    this->fillCipherCombo();

    this->lastDir = Utils::userApplicationDataDirectory();

    this->resetFields();

    // Fenster füllen
    ui->tabWidget->setCurrentIndex(0);
    this->fillFieldFromConfig();


    // Mittig ausrichten
    int winW = this->width();
    int winH = this->height();

    int left (0);
    int top (0);
    QRect geom (0,0,0,0);
   {
        // Desktop auswerten
        geom = qApp->primaryScreen()->geometry();
        top = geom.height();
        left = geom.width();

        // wenn ich das richtig verstanden habe muss man die virtuellen desktops nicht umrechnen

        // Die Breite bei virtuellen Desktops vierteln
        //if (left > 2000 && qApp->desktop()->isVirtualDesktop()) {
        //    left /= 4;
        //} else {
            // Normaler Desktop
            left = (left - winH) / 2;
        //}
        // Height
       // if (top > 2000 && qApp->desktop()->isVirtualDesktop()) {
       //     top /= 4;
       // } else {
            top = (top - winH) / 2;
       // }
    }
    // Nun die neuen setzen
    this->setGeometry(left, top, winW, winH);
    // Öffnen
    e->accept();
    this->setWindowState(Qt::WindowActive);
}

void AdvConfigSettingsWidget::on_cmdClose_clicked()
{
    this->close();
}

void AdvConfigSettingsWidget::fillCipherCombo() {
    ui->cmbChiper->insertItem(0, QLatin1String("Standard"));
    ui->cmbChiper->insertItem(1, QLatin1String("DES-CFB"));
    ui->cmbChiper->insertItem(2, QLatin1String("DES-CBC"));
    ui->cmbChiper->insertItem(3, QLatin1String("RC2-CBC"));
    ui->cmbChiper->insertItem(4, QLatin1String("RC2-CFB"));
    ui->cmbChiper->insertItem(5, QLatin1String("RC2-OFB"));
    ui->cmbChiper->insertItem(6, QLatin1String("DES-EDE-CBC"));
    ui->cmbChiper->insertItem(7, QLatin1String("DES-EDE3-CBC"));
    ui->cmbChiper->insertItem(8, QLatin1String("DES-OFB"));
    ui->cmbChiper->insertItem(9, QLatin1String("DES-EDE-CFB"));
    ui->cmbChiper->insertItem(10, QLatin1String("DES-EDE3-CFB"));
    ui->cmbChiper->insertItem(11, QLatin1String("DES-EDE-OFB"));
    ui->cmbChiper->insertItem(12, QLatin1String("DES-EDE3-OFB"));
    ui->cmbChiper->insertItem(13, QLatin1String("DESX-CBC"));
    ui->cmbChiper->insertItem(14, QLatin1String("BF-CBC"));
    ui->cmbChiper->insertItem(15, QLatin1String("BF-CFB"));
    ui->cmbChiper->insertItem(16, QLatin1String("BF-OFB"));
    ui->cmbChiper->insertItem(17, QLatin1String("RC2-40-CBC"));
    ui->cmbChiper->insertItem(18, QLatin1String("CAST5-CBC"));
    ui->cmbChiper->insertItem(19, QLatin1String("CAST5-CFB"));
    ui->cmbChiper->insertItem(20, QLatin1String("CAST5-OFB"));
    ui->cmbChiper->insertItem(21, QLatin1String("RC2-64-CBC"));
    ui->cmbChiper->insertItem(22, QLatin1String("AES-128-CBC"));
    ui->cmbChiper->insertItem(23, QLatin1String("AES-128-OFB"));
    ui->cmbChiper->insertItem(24, QLatin1String("AES-128-CFB"));
    ui->cmbChiper->insertItem(25, QLatin1String("AES-192-CBC"));
    ui->cmbChiper->insertItem(26, QLatin1String("AES-192-OFB"));
    ui->cmbChiper->insertItem(27, QLatin1String("AES-192-CFB"));
    ui->cmbChiper->insertItem(28, QLatin1String("AES-256-CBC"));
    ui->cmbChiper->insertItem(29, QLatin1String("AES-256-OFB"));
    ui->cmbChiper->insertItem(30, QLatin1String("AES-256-CFB"));
    ui->cmbChiper->insertItem(31, QLatin1String("AES-128-CFB1"));
    ui->cmbChiper->insertItem(32, QLatin1String("AES-192-CFB1"));
    ui->cmbChiper->insertItem(33, QLatin1String("AES-256-CFB1"));
    ui->cmbChiper->insertItem(34, QLatin1String("AES-128-CFB8"));
    ui->cmbChiper->insertItem(35, QLatin1String("AES-192-CFB8"));
    ui->cmbChiper->insertItem(36, QLatin1String("AES-256-CFB8"));
    ui->cmbChiper->insertItem(37, QLatin1String("DES-CFB1"));
    ui->cmbChiper->insertItem(38, QLatin1String("DES-CFB8"));
}

int AdvConfigSettingsWidget::getIndexFromCipher(QString cipher) {
    if (cipher == QLatin1String("DES-CFB"))
        return 1;
    else if (cipher == QLatin1String("DES-CBC"))
        return 2;
    else if (cipher == QLatin1String("RC2-CBC"))
        return 3;
    else if (cipher == QLatin1String("RC2-CFB"))
        return 4;
    else if (cipher == QLatin1String("RC2-OFB"))
        return 5;
    else if (cipher == QLatin1String("DES-EDE-CBC"))
        return 6;
    else if (cipher == QLatin1String("DES-EDE3-CBC"))
        return 7;
    else if (cipher == QLatin1String("DES-OFB"))
        return 8;
    else if (cipher == QLatin1String("DES-EDE-CFB"))
        return 9;
    else if (cipher == QLatin1String("DES-EDE3-CFB"))
        return 10;
    else if (cipher == QLatin1String("DES-EDE-OFB"))
        return 11;
    else if (cipher == QLatin1String("DES-EDE3-OFB"))
        return 12;
    else if (cipher == QLatin1String("DESX-CBC"))
        return 13;
    else if (cipher == QLatin1String("BF-CBC"))
        return 14;
    else if (cipher == QLatin1String("BF-CFB"))
        return 15;
    else if (cipher == QLatin1String("BF-OFB"))
        return 16;
    else if (cipher == QLatin1String("RC2-40-CBC"))
        return 17;
    else if (cipher == QLatin1String("CAST5-CBC"))
        return 18;
    else if (cipher == QLatin1String("CAST5-CFB"))
        return 19;
    else if (cipher == QLatin1String("CAST5-OFB"))
        return 20;
    else if (cipher == QLatin1String("RC2-64-CBC"))
        return 21;
    else if (cipher == QLatin1String("AES-128-CBC"))
        return 22;
    else if (cipher == QLatin1String("AES-128-OFB"))
        return 23;
    else if (cipher == QLatin1String("AES-128-CFB"))
        return 24;
    else if (cipher == QLatin1String("AES-192-CBC"))
        return 25;
    else if (cipher == QLatin1String("AES-192-OFB"))
        return 26;
    else if (cipher == QLatin1String("AES-192-CFB"))
        return 27;
    else if (cipher == QLatin1String("AES-256-CBC"))
        return 28;
    else if (cipher == QLatin1String("AES-256-OFB"))
        return 29;
    else if (cipher == QLatin1String("AES-256-CFB"))
        return 30;
    else if (cipher == QLatin1String("AES-128-CFB1"))
        return 31;
    else if (cipher == QLatin1String("AES-192-CFB1"))
        return 32;
    else if (cipher == QLatin1String("AES-256-CFB1"))
        return 33;
    else if (cipher == QLatin1String("AES-128-CFB8"))
        return 34;
    else if (cipher == QLatin1String("AES-192-CFB8"))
        return 35;
    else if (cipher == QLatin1String("AES-256-CFB8"))
        return 36;
    else if (cipher == QLatin1String("DES-CFB1"))
        return 37;
    else if (cipher == QLatin1String("DES-CFB8"))
        return 38;
    else
        return 0;
}

void AdvConfigSettingsWidget::resetFields() {
    ui->cbClient->setChecked(false);
    ui->cbFloat->setChecked(false);
    ui->cbCompLzo->setChecked(false);
    ui->cbNobind->setChecked(false);
    ui->cbPersistKey->setChecked(false);
    ui->cbPersistTun->setChecked(false);
    ui->cbNoCache->setChecked(false);
    ui->cbRedirectGateway->setChecked(false);

    ui->cmbDev->setCurrentIndex(0);
    ui->txtMtu->setText("");

    //ui->txtRemote->setText("");
    //ui->txtRemotePort->setText("");
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
    ui->cbBlockOutsideDns->setChecked(false);
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

    ui->cmbProxyAuth->setCurrentIndex(0);
    ui->cmbAuthMethod->setCurrentIndex(0);

    ui->cbMuteWirelessWarning->setChecked(false);

    ui->cmbChiper->setCurrentIndex(0);

    ui->txtAfterConnect->setText("");
    ui->txtAfterDisconnect->setText("");
    ui->txtBeforeConnect->setText("");
    ui->txtBeforeDisconnect->setText("");
    ui->txtErrorConnect->setText("");

    ui->txtScriptACDelay->setText("5000");
}

void AdvConfigSettingsWidget::fillFieldFromConfig() {
    // Load Config Data
    QString cFile (this->configObj->GetConfigPath());
    QFile cf (cFile);

    if (!cf.open(QIODevice::ReadOnly | QIODevice::Text)) {
        Message::error(QObject::tr("Unable to read file!"), QObject::tr("Load Configuration"), this);
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
    bool isBlockOutsideDnsChecked = false;
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
    bool isPkcs12Checked = false;

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

        if (!isCAChecked)
        {
            if (line.left(2).toUpper().trimmed() == "CA")
            {
                isCAChecked = true;
                QString tmpLine (line.trimmed());
                // Key value
                tmpLine = tmpLine.right(line.size() - 2);
                // Remove " with nothing
                tmpLine = tmpLine.replace("\"", "");
                // Trim the line
                tmpLine = tmpLine.trimmed();
                //
                ui->txtCA->setText(tmpLine);
                //
                this->caValue = tmpLine;
            }
            if (line.startsWith("<ca>", Qt::CaseInsensitive))
            {
                ui->txtCA->setText("inline");

                this->caValue = "inline";

                while (!line.startsWith("</ca>", Qt::CaseInsensitive))
                {
                    line = in.readLine();

                    if(!line.startsWith("</ca>", Qt::CaseInsensitive) && !line.isEmpty())
                    {
                        if (line.startsWith("-----END CERTIFICATE-----", Qt::CaseInsensitive))
                        {
                              caValueInline += line;
                        }
                        else
                        {
                            caValueInline += line + QLatin1String("\n");
                        }
                    }
                }
            }
        }

        if (!isPkcs12Checked) {
            if (line.left(6).toUpper().trimmed() == "PKCS12") {
                isPkcs12Checked = true;
                QString tmpLine (line.trimmed());
                // Key value
                tmpLine = tmpLine.right(line.size() - 6);
                // Remove " with nothing
                tmpLine = tmpLine.replace("\"", "");
                // Trim the line
                tmpLine = tmpLine.trimmed();
                //
                ui->txtPkcs12Path->setText(tmpLine);
                //
                this->pkcs12Value = tmpLine;
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

        if (!isBlockOutsideDnsChecked){
                if (line.toUpper().trimmed() == "BLOCK-OUTSIDE-DNS") {
                    isBlockOutsideDnsChecked = true;
                    ui->cbBlockOutsideDns->setChecked(true);
                } else {
                    ui->cbBlockOutsideDns->setChecked(false);
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
                if (line.trimmed().toUpper().left(7) == "REMOTE ") {
                    //isRemoteChecked = true;
                    QStringList keyvalList = line.split(" ");
                    if (keyvalList.size() < 3){
                        //ui->txtRemotePort->setText("");
                        //ui->txtRemote->setText("");
                    } else {
                        QString val = keyvalList[1];
                        QString valport = keyvalList[2];
                        //ui->txtRemote->setText(val.replace("\"",""));
                        //ui->txtRemotePort->setText(valport.replace("\"",""));
                        QList<QStandardItem*> newRow;
                        QStandardItemModel * m = (QStandardItemModel*)ui->lvRemote->model();
                        QStandardItem* itm = new QStandardItem(val.replace("\"",""));
                        newRow.append(itm);
                        itm = new QStandardItem(valport.replace("\"",""));
                        newRow.append(itm);

                        // Check if we got proto settings
                        if (keyvalList.size() == 4) {
                            QString proto = keyvalList[3];
                            // If its no tcp it is udp ...
                            if(proto.trimmed().toUpper() == "TCP") {
                                ui->cmbProto->setCurrentIndex(1);
                            } else {
                                ui->cmbProto->setCurrentIndex(2);
                            }
                        }

                        m->appendRow(newRow);
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
        if (!isCertChecked)
        {
            if (line.left(4).toUpper().trimmed() == "CERT")
            {
                    isCertChecked = true;
                    QString tmpLine (line.trimmed());
                    // Key value
                    tmpLine = tmpLine.right(line.size() - 4);
                    // Remove " with nothing
                    tmpLine = tmpLine.replace("\"", "");
                    // Trim the line
                    tmpLine = tmpLine.trimmed();
                    //
                    ui->txtCert->setText(tmpLine);
                    //
                    this->certValue = tmpLine;
            }
            if (line.startsWith("<cert>", Qt::CaseInsensitive))
            {
                ui->txtCert->setText("inline");

                this->certValue = "inline";

                while (!line.startsWith("</cert>", Qt::CaseInsensitive))
                {
                    line = in.readLine();

                    if(!line.startsWith("</cert>", Qt::CaseInsensitive) && !line.isEmpty())
                    {
                        if (line.startsWith("-----END CERTIFICATE-----", Qt::CaseInsensitive))
                        {
                              certValueInline += line;
                        }
                        else
                        {
                            certValueInline += line + QLatin1String("\n");
                        }
                    }
                }
             }
         }
        if (!isKeyChecked)
        {
            if (line.left(3).toUpper().trimmed() == "KEY")
            {
                isKeyChecked = true;
                QString tmpLine (line.trimmed());
                // Key value
                tmpLine = tmpLine.right(line.size() - 3);
                // Remove " with nothing
                tmpLine = tmpLine.replace("\"", "");
                // Trim the line
                tmpLine = tmpLine.trimmed();
                //
                ui->txtKey->setText(tmpLine);
                //
                this->keyValue = tmpLine;
             }
            if (line.startsWith("<key>", Qt::CaseInsensitive))
            {
               ui->txtKey->setText("inline");

               this->keyValue = "inline";

               while (!line.startsWith("</key>", Qt::CaseInsensitive))
               {
                   line = in.readLine();

                   if(!line.startsWith("</key>", Qt::CaseInsensitive) && !line.isEmpty())
                   {
                       if (line.startsWith("-----END PRIVATE KEY-----", Qt::CaseInsensitive))
                       {
                             keyValueInline += line;
                       }
                       else
                       {
                           keyValueInline += line + QLatin1String("\n");
                       }
                   }
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
                    ui->txtHTTPIP->setText("");
                    ui->txtHTTPPort->setText("");
                    ui->cmbAuthMethod->setCurrentIndex(0);
                    ui->cmbProxyAuth->setCurrentIndex(0);

                    if (keyvalList.size() > 1) {
                        QString val (keyvalList.at(1));
                        ui->txtHTTPIP->setText(val.replace("\"",""));
                    }

                    if (keyvalList.size() > 2) {
                        QString val (keyvalList.at(2));
                        ui->txtHTTPPort->setText(val.replace("\"",""));
                    }

                    if (keyvalList.size() > 3) {
                        QString type (keyvalList.at(3));
                        type = type.replace("\"","").toUpper();
                        if (type == QLatin1String("STDIN")) {
                            ui->cmbProxyAuth->setCurrentIndex(1);
                        } else if (type == QLatin1String("AUTO")) {
                            ui->cmbProxyAuth->setCurrentIndex(2);
                        } else if (type == QLatin1String("AUTO-NCT")) {
                            ui->cmbProxyAuth->setCurrentIndex(3);
                        }
                    }

                    if (keyvalList.size() > 4) {
                        QString type (keyvalList.at(4));
                        type = type.replace("\"","").toUpper();
                        if (type == QLatin1String("BASIC")) {
                            ui->cmbAuthMethod->setCurrentIndex(1);
                        } else if (type == QLatin1String("NTLM")) {
                            ui->cmbAuthMethod->setCurrentIndex(2);
                        }
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

   ui->txtAfterConnect->setText(this->configObj->getScript ("AC"));
   ui->txtAfterDisconnect->setText(this->configObj->getScript ("AD"));
   ui->txtBeforeConnect->setText(this->configObj->getScript ("BC"));
   ui->txtBeforeDisconnect->setText(this->configObj->getScript ("BD"));
   ui->txtErrorConnect->setText(this->configObj->getScript ("EC"));
   ui->txtScriptACDelay->setText((this->configObj->getScript ("TO") == "" ? "5000" : this->configObj->getScript ("TO")));

   // Pkcs 12 gefunden dann dies setzen
   if (isPkcs12Checked) {
       ui->rbPkcs->setChecked(true);
       this->on_rbPkcs_toggled(true);
   } else {
       ui->rbNormal->setChecked(true);
       this->on_rbNormal_toggled(true);
   }
}

void AdvConfigSettingsWidget::on_cmdSave_clicked()
{


    QStringList out;
    // Datei offen, fertig zum schreiben
    out << QLatin1String("##############################################\n");
    out << QLatin1String("### \n");
    out << QLatin1String("### Configuration file created by Securepoint SSL VPN ") + QDate::currentDate().toString() + QLatin1String(" - ") + QTime::currentTime().toString() +  QLatin1String("\n");
    out << QLatin1String("### Project website: http://sourceforge.net/projects/securepoint/ \n");
    out << QLatin1String("### Securepoint GmbH, Bleckeder Landstrasse 28, 21337 Lueneburg, Germany; www.securepoint.de \n");
    out << QLatin1String("### \n");
    out << QLatin1String("### For further information about the configuration file, \n");
    out << QLatin1String("### please visit: http://www.openvpn.net/index.php/open-source/documentation\n");
    out << QLatin1String("### \n");
    out << QLatin1String("##############################################\n\n");
    if (ui->cbClient->isChecked())
        out << QLatin1String("client\n");

    if (ui->cbFloat->isChecked())
        out << QLatin1String("float\n");

    if (ui->cbCompLzo->isChecked())
        out << QLatin1String("comp-lzo\n");

    if (ui->cbFloat->isChecked())
        out << QLatin1String("float\n");

    if (ui->cbNobind->isChecked())
        out << QLatin1String("nobind\n");

    if (ui->cbPersistKey->isChecked())
        out << QLatin1String("persist-key\n");

    if (ui->cbPersistTun->isChecked())
        out << QLatin1String("persist-tun\n");

    if (ui->cbNoCache->isChecked())
        out << QLatin1String("auth-nocache\n");

    if (ui->cmbDev->currentIndex() != 0) {
        if (ui->cmbDev->currentIndex() == 1)
            out << QLatin1String("dev tun\n");
        else
            out << QLatin1String("dev tap\n");
    }

    if (!ui->txtMtu->text().isEmpty()) {
        out << QLatin1String("tun-mtu ") << ui->txtMtu->text() << QLatin1String("\n");
    }

    //out << QLatin1String("remote ") << ui->txtRemote->text() << QLatin1String(" ") + ui->txtRemotePort->text() << QLatin1String("\n");
    auto remoteList = [&]() -> QStringList
    {
        QStringList list;
        QStandardItemModel * m = (QStandardItemModel*)this->ui->lvRemote->model();
        for(int row = 0; row < m->rowCount(); ++row)
        {
            list.append(m->item(row, 0)->text() + QLatin1String(" ") +  m->item(row , 1)->text());
        }
        return list;
    };

    QVariant t;
    for(const auto remote : remoteList())
    {
        out << QLatin1String("remote ") << remote << QLatin1String("\n");
    }

    if (ui->cmbProto->currentIndex() != 0){
        if (ui->cmbProto->currentIndex() == 1)
            out << QLatin1String("proto tcp\n");
        else
            out << QLatin1String("proto udp\n");
    }

    /*if(!Settings::getInstance()->getIsPortableClient())*/ {
        if (ui->rbNormal->isChecked() && !ui->txtCA->text().isEmpty())
        {
            // If temp value and dialog value are not equals, copy the new certificate to the ovpn dir
            // Otherwise write the plain data from dialog t the config
            if (this->caValue != ui->txtCA->text() && ui->txtCA->text() != "inline")
            {
                // Copy file to config dir and set new path
                QString newPath (ConfigValues::instance()->pathOfFile(this->configObj->GetConfigPath()));
                QString nameOfFile (ConfigValues::instance()->fileNameOfAbsolutePath(ui->txtCA->text()));
                QFile::copy(ui->txtCA->text(), QString("%1%2")
                                                .arg((newPath.right(1) == QLatin1String("/") ? newPath : QString("%1/").arg(newPath)))
                                                .arg(nameOfFile));
                // Set new certificate location
                out << QLatin1String("ca \"") << nameOfFile << QLatin1String("\"\n");
            } else if(ui->txtCA->text() != "inline")
            {
                // Set the origin data
                out << QLatin1String("ca \"") << ui->txtCA->text().replace("/", "\\\\") << QLatin1String("\"\n");
            }
        }


        if (ui->rbNormal->isChecked() && !ui->txtCert->text().isEmpty()) {
            // If temp value and dialog value are not equals, copy the new certificate to the ovpn dir
            // Otherwise write the plain data from dialog t the config
            if (this->certValue != ui->txtCert->text() && ui->txtCert->text() != "inline") {
                // Copy file to config dir and set new path
                QString newPath (ConfigValues::instance()->pathOfFile(this->configObj->GetConfigPath()));
                QString nameOfFile (ConfigValues::instance()->fileNameOfAbsolutePath(ui->txtCert->text()));
                QFile::copy(ui->txtCert->text(), QString("%1%2")
                                                .arg((newPath.right(1) == QLatin1String("/") ? newPath : QString("%1/").arg(newPath)))
                                                .arg(nameOfFile));
                // Set new certificate location
                out << QLatin1String("cert \"") << nameOfFile << QLatin1String("\"\n");
            } else if(ui->txtCert->text() != "inline")
            {
                // Set the origin data
                out << QLatin1String("cert \"") << ui->txtCert->text().replace("/", "\\\\") << QLatin1String("\"\n");
            }
        }


        if (ui->rbNormal->isChecked() && !ui->txtKey->text().isEmpty())
        {
            // If temp value and dialog value are not equals, copy the new certificate to the ovpn dir
            // Otherwise write the plain data from dialog to the config
            if (this->keyValue != ui->txtKey->text() && ui->txtKey->text() != "inline")
            {
                // Copy file to config dir and set new path
                QString newPath (ConfigValues::instance()->pathOfFile(this->configObj->GetConfigPath()));
                QString nameOfFile (ConfigValues::instance()->fileNameOfAbsolutePath(ui->txtKey->text()));
                QFile::copy(ui->txtKey->text(), QString("%1%2")
                                                .arg((newPath.right(1) == QLatin1String("/") ? newPath : QString("%1/").arg(newPath)))
                                                .arg(nameOfFile));
                // Set new certificate location
                out << QLatin1String("key \"") << nameOfFile << QLatin1String("\"\n");
            } else if(ui->txtKey->text() != "inline")
            {
                // Set the origin data
                out << QLatin1String("key \"") << ui->txtKey->text().replace("/", "\\\\") << QLatin1String("\"\n");
            }
        }


        if (ui->rbPkcs->isChecked() && !ui->txtPkcs12Path->text().isEmpty()) {
            // If temp value and dialog value are not equals, copy the new certificate to the ovpn dir
            // Otherwise write the plain data from dialog t the config
            if (this->pkcs12Value != ui->txtPkcs12Path->text()) {
                // Copy file to config dir and set new path
                QString newPath (ConfigValues::instance()->pathOfFile(this->configObj->GetConfigPath()));
                QString nameOfFile (ConfigValues::instance()->fileNameOfAbsolutePath(ui->txtPkcs12Path->text()));
                QFile::copy(ui->txtPkcs12Path->text(), QString("%1%2")
                                                .arg((newPath.right(1) == QLatin1String("/") ? newPath : QString("%1/").arg(newPath)))
                                                .arg(nameOfFile));
                // Set new certificate location
                out << QLatin1String("pkcs12 \"") << nameOfFile << QLatin1String("\"\n");
            } else {
                // Set the origin data
                out << QLatin1String("pkcs12 ") << ui->txtPkcs12Path->text() << QLatin1String("\n");
            }

        }
    }

    if (ui->rbNormal->isChecked() && ui->cbCertIsServer->isChecked())
        out << QLatin1String("ns-cert-type server\n");

    if (!ui->txtLinuxUser->text().isEmpty())
        out << QLatin1String("user ") << ui->txtLinuxUser->text() << QLatin1String("\n");

    if (!ui->txtLinuxGroup->text().isEmpty())
        out << QLatin1String("group ") << ui->txtLinuxGroup->text() << QLatin1String("\n");

    if (ui->cbMsfix->isChecked())
        out << QLatin1String("mssfix\n");

    if (ui->cbBlockOutsideDns->isChecked()) {
        out << QLatin1String("block-outside-dns\n");
    }

    if (!ui->txtFragment->text().isEmpty())
        out << QLatin1String("fragment ") << ui->txtFragment->text() << QLatin1String("\n");

    if (ui->cmbRouteMethod->currentIndex() != 0){
        if (ui->cmbRouteMethod->currentIndex() == 1)
            out << QLatin1String("route-method exe\n");
        else
            out << QLatin1String("route-method ipapi\n");
    }

    if (!ui->txtDevNode->text().isEmpty())
        out << QLatin1String("dev-node ") << ui->txtDevNode->text() << QLatin1String("\n");

    if (!ui->txtVerbose->text().isEmpty())
        out << QLatin1String("verb ") << ui->txtVerbose->text() << QLatin1String("\n");

    if (!ui->txtRouteDelay->text().isEmpty())
        out << QLatin1String("route-delay ") << ui->txtRouteDelay->text() << QLatin1String("\n");

    if (!ui->txtMute->text().isEmpty())
        out << QLatin1String("mute ") << ui->txtMute->text() << QLatin1String("\n");

    if (ui->cbRandomHost->isChecked())
        out << QLatin1String("remote-random\n");

    if (ui->cbResolveHostname->isChecked())
        out << QLatin1String("resolve-retry infinite\n");

    if (ui->cbHTTPRetry->isChecked())
        out << QLatin1String("http-proxy-retry\n");

    if (!ui->txtHTTPIP->text().isEmpty()) {
        QString type;
        if (ui->cmbProxyAuth->currentIndex() != 0) {
            if (ui->cmbProxyAuth->currentIndex() == 1) {
                type = QLatin1String("stdin");
            } else if (ui->cmbProxyAuth->currentIndex() == 2) {
                type = QLatin1String("auto");
            } else if (ui->cmbProxyAuth->currentIndex() == 3) {
                type = QLatin1String("auto-nct");
            }
        }

        QString method;
        if (ui->cmbAuthMethod->currentIndex() != 0) {
            if (ui->cmbAuthMethod->currentIndex() == 1) {
                method = QLatin1String("basic");
            } else if (ui->cmbAuthMethod->currentIndex() == 2) {
                method = QLatin1String("ntlm");
            }
        }
        out << QLatin1String("http-proxy ") << ui->txtHTTPIP->text() << QLatin1String(" ") << ui->txtHTTPPort->text() << (!type.isEmpty() ? QLatin1String(" ") : QLatin1String("")) << type << (!method.isEmpty() ? QLatin1String(" ") : QLatin1String("")) << method << QLatin1String("\n");
    }

    if (ui->cbMuteWirelessWarning->isChecked())
        out << QLatin1String("mute-replay-warnings\n");

    if (ui->cmbChiper->currentIndex() != 0)
        out << QLatin1String("cipher ") << ui->cmbChiper->currentText() << QLatin1String("\n");

    if (ui->cbRedirectGateway->isChecked())
        out << QLatin1String("redirect-gateway\n");

    if (ui->txtCA->text() == "inline")
    {
        // check for <tag>
        QString target("<ca>");
        if(!caValueInline.contains(target,Qt::CaseInsensitive))
        {
            caValueInline = target + "\n" + caValueInline;
        }

        out << caValueInline;

        // check for </tag>
        target = ("</ca>");
        if(!caValueInline.contains(target,Qt::CaseInsensitive))
        {
            out << "\n" + target;
        }

        out << "\n";
    }
    if (ui->txtCert->text() == "inline")
    {
        // check for <tag>
        QString target("<cert>");
        if(!certValueInline.contains(target,Qt::CaseInsensitive))
        {
            certValueInline = target + "\n" + certValueInline;
        }

        out << certValueInline;

        // check for </tag>
        target = ("</cert>");
        if(!certValueInline.contains(target,Qt::CaseInsensitive))
        {
            out << "\n" + target;
        }

        out << "\n";
    }
    if (ui->txtKey->text() == "inline")
    {
        // check for <tag>
        QString target("<key>");
        if(!keyValueInline.contains(target,Qt::CaseInsensitive))
        {
            keyValueInline = target + "\n" + keyValueInline;
        }

        out << keyValueInline;

        // check for </tag>
        target = ("</key>");
        if(!keyValueInline.contains(target,Qt::CaseInsensitive))
        {
            out << "\n" + target;
        }

        out <<  "\n";
    }

    QStringList oldFields = this->getAllFieldWhichNotIntoTheInterface();

    if (oldFields.size() > 0) {
       // Alte Felder die nicht in der GUI sind vorhanden
        out << QLatin1String("\n#Fields which could be not manage wih GUI \n\n");
        out << oldFields.join("\n");
    }



    // Write the string list to file
    QFile cf (this->configObj->GetConfigPath());

    if (!cf.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        Message::error(QObject::tr("Unable to open file!"), QObject::tr("Save Configuration"), this);
        return;
    }

    QTextStream configStream(&cf);

    //
    QString content;
    //
    for ( QStringList::Iterator it = out.begin(); it != out.end(); ++it ) {
        content += QString (*it);
    }

    configStream << content + "\n";

    cf.waitForBytesWritten(300);

    //
    cf.close();

    // Update checksum
    // Update the checksum of the file
    Checksum::instance()->createNewChecksum(Checksum::instance()->base64OfContent(content), this->configObj->GetId(), this->configObj->GetConfigPath());

    // Scripts ablegen
    bool scriptDataWrite(false);
#if 1
    QFile sf (this->configObj->GetDir() + QLatin1String("/scripts.conf"));

    if (!sf.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        Message::error(QObject::tr("Unable to open file!"), QObject::tr("Save scripts"), this);
        return;
    }

    QTextStream sout(&sf);
    if (!ui->txtAfterConnect->text().isEmpty()) {
        sout << QLatin1String("AC:") << ui->txtAfterConnect->text() << QLatin1String("\n");
        if (!ui->txtScriptACDelay->text().isEmpty()) {
            sout << QLatin1String("TO:") << ui->txtScriptACDelay->text() << QLatin1String("\n");
        }

        scriptDataWrite = true;
    }

    if (!ui->txtAfterDisconnect->text().isEmpty()) {
        sout << QLatin1String("AD:") << ui->txtAfterDisconnect->text() << QLatin1String("\n");
        scriptDataWrite = true;
    }

    if (!ui->txtBeforeConnect->text().isEmpty()) {
        sout << QLatin1String("BC:") << ui->txtBeforeConnect->text() << QLatin1String("\n");
        scriptDataWrite = true;
    }

    if (!ui->txtBeforeDisconnect->text().isEmpty()) {
        sout << QLatin1String("BD:") << ui->txtBeforeDisconnect->text() << QLatin1String("\n");
        scriptDataWrite = true;
    }

    if (!ui->txtErrorConnect->text().isEmpty()) {
        sout << QLatin1String("EC:") << ui->txtErrorConnect->text() << QLatin1String("\n");
        scriptDataWrite = true;
    }

    // Daten geschrieben?
    if (!scriptDataWrite) {
        if (!sf.remove()) {
            Message::error(QObject::tr("Can't delete scriptfile!"), QObject::tr("Save scripts"), this);
            return;
        }
    }
    sf.close();
#endif

    this->close();
}

void AdvConfigSettingsWidget::on_cmdGetCAPath_clicked()
{
    // todo inline
    QFileDialog caFileDialog;
    QString filename = caFileDialog.getOpenFileName(this, QObject::tr("Find root ca"), this->lastDir, QObject::tr("Certificates (*.crt *.cert *.pem);;All files (*.*)"));
    if (!filename.isEmpty()) {
        int lastSlash = filename.lastIndexOf("/");
        QString filepath = filename.left(lastSlash);
        this->lastDir = filepath;
        if (filepath.replace("\\", "/").toLower() == this->configObj->GetDir().toLower().replace('\\', '/'))
            ui->txtCA->setText(filename.right(filename.size() - lastSlash -1));
        else
            ui->txtCA->setText(filename);
    }
}

void AdvConfigSettingsWidget::setPathInline()
{
    switch(pathClicked){
    case 1:
        ui->txtCA->setText("inline");
        break;
    case 2:
        ui->txtCert->setText("inline");
        break;
    case 3:
        ui->txtKey->setText("inline");
        break;
    }
}

void AdvConfigSettingsWidget::on_cmdSetCaInline_clicked()
{
    pathClicked = 1;

    QString cPath (this->configObj->GetConfigPath());
    InlineWindow widget("ca", caValueInline, p_caValueInline, this);
    //widget.setParent(this);

    widget.exec();

}

void AdvConfigSettingsWidget::on_cmdGetCertPath_clicked()
{
    // todo inline
    QFileDialog certFileDialog;
    QString filename = certFileDialog.getOpenFileName(this, QObject::tr("Find certificates"), this->lastDir, QObject::tr("Certificates (*.cert *.pem);;All files (*.*)"));
    if (!filename.isEmpty()) {
        int lastSlash = filename.lastIndexOf("/");
        QString filepath = filename.left(lastSlash);
        this->lastDir = filepath;
        if (filepath.replace("\\", "/").toLower() == this->configObj->GetDir().toLower().replace("\\", "/"))
            ui->txtCert->setText(filename.right(filename.size() - lastSlash -1));
        else
            ui->txtCert->setText(filename);
    }
}

void AdvConfigSettingsWidget::on_cmdSetCertInline_clicked()
{
    pathClicked = 2;

    QString cPath (this->configObj->GetConfigPath());
    InlineWindow widget("cert", certValueInline, p_certValueInline, this);

    widget.exec();
}

void AdvConfigSettingsWidget::on_cmdGetKeyPath_clicked()
{
    // todo inline
    QFileDialog keyFileDialog;
    QString filename = keyFileDialog.getOpenFileName(this, QObject::tr("Find key files"), this->lastDir, QObject::tr("Key files (*.key *.pem);;All files (*.*)"));
    if (!filename.isEmpty()) {
        int lastSlash = filename.lastIndexOf("/");
        QString filepath (filename.left(lastSlash));
        this->lastDir = filepath;
        if (filepath.replace("\\", "/").toLower() == this->configObj->GetDir().toLower().replace("\\", "/"))
            ui->txtKey->setText(filename.right(filename.size() - lastSlash -1));
        else
            ui->txtKey->setText(filename);
    }
}

void AdvConfigSettingsWidget::on_cmdSetKeyInline_clicked()
{
    pathClicked = 3;

    QString cPath (this->configObj->GetConfigPath());
    InlineWindow widget("key", keyValueInline,  p_keyValueInline, this);

    widget.exec();
}

void AdvConfigSettingsWidget::on_cmdBeforeConnect_clicked()
{
    QFileDialog keyFileDialog;
    QString filename = keyFileDialog.getOpenFileName(this, QObject::tr("Find executeable files"), this->configObj->GetDir(), QObject::tr("All files (*.*)"));
    if (!filename.isEmpty()) {
            ui->txtBeforeConnect->setText(filename);
    }
}

void AdvConfigSettingsWidget::on_cmdAfterConnect_clicked()
{
    QFileDialog keyFileDialog;
    QString filename = keyFileDialog.getOpenFileName(this, QObject::tr("Find executeable files"), this->configObj->GetDir(), QObject::tr("All files (*.*)"));
    if (!filename.isEmpty()) {
            ui->txtAfterConnect->setText(filename);
    }
}

void AdvConfigSettingsWidget::on_cmdBeforeDisconnect_clicked()
{
    QFileDialog keyFileDialog;
    QString filename = keyFileDialog.getOpenFileName(this, QObject::tr("Find executeable files"), this->configObj->GetDir(), QObject::tr("All files (*.*)"));
    if (!filename.isEmpty()) {
            ui->txtBeforeDisconnect->setText(filename);
    }
}

void AdvConfigSettingsWidget::on_cmdAfterDisconnect_clicked()
{
    QFileDialog keyFileDialog;
    QString filename = keyFileDialog.getOpenFileName(this, QObject::tr("Find executeable files"), this->configObj->GetDir(), QObject::tr("All files (*.*)"));
    if (!filename.isEmpty()) {
            ui->txtAfterDisconnect->setText(filename);
    }
}

void AdvConfigSettingsWidget::on_cmdErrorConnect_clicked()
{
    QFileDialog keyFileDialog;
    QString filename = keyFileDialog.getOpenFileName(this, QObject::tr("Find executeable files"), this->configObj->GetDir(), QObject::tr("All files (*.*)"));
    if (!filename.isEmpty()) {
            ui->txtErrorConnect->setText(filename);
    }
}

QStringList AdvConfigSettingsWidget::getAllFieldWhichNotIntoTheInterface()
{
    // Load Config Data
    QStringList fieldsNotIncluded;
    QString cFile;
    fieldsNotIncluded.clear();
    cFile = this->configObj->GetConfigPath();
    QFile cf (cFile);

    if (!cf.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        Message::error(QObject::tr("Unable to read file!"), QObject::tr("Load Configuration"), this);
        return fieldsNotIncluded;
    }


    QTextStream in(&cf);

    while (!in.atEnd())
    {
        QString line = in.readLine();
        if (line.trimmed().left(3).toUpper() != "###"
                    && line.trimmed().replace("\n", "") != ""
                    && line.trimmed().left(13).toUpper() != "#FIELDS WHICH"
                    && line.toUpper().trimmed() != "CLIENT"
                    && line.trimmed().left(2).toUpper() != "CA"
                    && line.trimmed().left(6).toUpper() != "PKCS12"
                    && line.trimmed().left(3).toUpper() != "DEV"
                    && line.trimmed().left(7).toUpper() != "TUN-MTU"
                    && line.trimmed().left(8).toUpper() != "FRAGMENT"
                    && line.toUpper().trimmed() != "MSSFIX"
                    && line.toUpper().trimmed() != "BLOCK-OUTSIDE-DNS"
                    && line.toUpper().trimmed() != "REDIRECT-GATEWAY"
                    && line.trimmed().left(5).toUpper() != "PROTO"
                    && line.toUpper().trimmed() != "FLOAT"
                    && line.trimmed().left(7).toUpper() != "REMOTE "
                    && line.toUpper().trimmed() != "NOBIND"
                    && line.toUpper().trimmed() != "PERSIST-KEY"
                    && line.toUpper().trimmed() != "PERSIST-TUN"
                    && line.trimmed().left(4).toUpper() != "CERT"
                    && line.trimmed().left(3).toUpper() != "KEY"
                    && line.toUpper().trimmed() != "COMP-LZO"
                    && line.trimmed().left(4).toUpper() != "VERB"
                    && line.trimmed().left(4).toUpper() != "MUTE"
                    && line.toUpper().trimmed() != "AUTH-NOCACHE"
                    && line.trimmed().left(12).toUpper() != "ROUTE-METHOD"
                    && line.trimmed().left(11).toUpper() != "ROUTE-DELAY"
                    && line.trimmed().left(8).toUpper() != "DEV-NODE"
                    && line.trimmed().toUpper() != "REMOTE-RANDOM"
                    && line.trimmed().left(13).toUpper() != "RESOLVE-RETRY"
                    && line.trimmed().left(4).toUpper() != "USER"
                    && line.trimmed().left(5).toUpper() != "GROUP"
                    && !line.trimmed().left(16).contains("HTTP-PROXY-RETRY", Qt::CaseInsensitive)
                    && !line.trimmed().left(11).contains("HTTP-PROXY ", Qt::CaseInsensitive)
                    && line.toUpper().trimmed() != "MUTE-REPLAY-WARNINGS"
                    && line.trimmed().left(6).toUpper() != "CIPHER"
                    && line.toUpper().trimmed() != "NS-CERT-TYPE SERVER")
        {

            /*
            if(line.startsWith("<ca>", Qt::CaseInsensitive) && ui->txtCA->text() != "inline")
            {
                caValueInline = "";
                while (!line.startsWith("</ca>", Qt::CaseInsensitive))
                {
                    line = (in.readLine());
                    caValueInline += line;
                }
            }
            else if(line.startsWith("<cert>", Qt::CaseInsensitive) && ui->txtCert->text() != "inline")
            {
                certValueInline = "";
                while (!line.startsWith("</cert>", Qt::CaseInsensitive))
                {
                    line = (in.readLine());
                    certValueInline += line;

                }
            }
            else if(line.startsWith("<key>", Qt::CaseInsensitive) && ui->txtKey->text() != "inline")
            {
                keyValueInline = "";
                while (!line.startsWith("</key>", Qt::CaseInsensitive))
                {
                    line = (in.readLine());
                    keyValueInline += line;

                }
            }
            */

            if(line.startsWith("<ca>", Qt::CaseInsensitive))
            {
                while (!line.startsWith("</ca>", Qt::CaseInsensitive))
                {
                    line = (in.readLine());
                }
            }
            else if(line.startsWith("<cert>", Qt::CaseInsensitive))
            {
                while (!line.startsWith("</cert>", Qt::CaseInsensitive))
                {
                    line = (in.readLine());
                }
            }
            else if(line.startsWith("<key>", Qt::CaseInsensitive))
            {
                while (!line.startsWith("</key>", Qt::CaseInsensitive))
                {
                    line = (in.readLine());
                }
            }

            else
            {
                    fieldsNotIncluded.append(line);
            }
        }
    }

    cf.close();
    return fieldsNotIncluded;
}

void AdvConfigSettingsWidget::on_rbPkcs_toggled(bool checked)
{
    if (checked) {
        ui->txtPkcs12Path->setEnabled(true);
        ui->cmdGetPkcs12Path->setEnabled(true);
        // Die normalen deaktivieren
        ui->txtKey->setEnabled(false);
        ui->txtCA->setEnabled(false);
        ui->txtCert->setEnabled(false);

        ui->cbCertIsServer->setEnabled(false);
        ui->cmdGetCAPath->setEnabled(false);
        ui->cmdGetCertPath->setEnabled(false);
        ui->cmdGetKeyPath->setEnabled(false);
    }
}

void AdvConfigSettingsWidget::on_cmdGetPkcs12Path_clicked()
{
    QFileDialog caFileDialog;
    QString filename = caFileDialog.getOpenFileName(this, QObject::tr("Find pkcs12"), this->lastDir, QObject::tr("Pkcs12 (*.p12)"));
    if (!filename.isEmpty()) {
        int lastSlash = filename.lastIndexOf("/");
        QString filepath = filename.left(lastSlash);
        this->lastDir = filepath;
        if (filepath.replace("\\", "/").toLower() == this->configObj->GetDir().toLower().replace("\\", "/"))
            ui->txtPkcs12Path->setText(filename.right(filename.size() - lastSlash -1));
        else
            ui->txtPkcs12Path->setText(filename);
    }
}

void AdvConfigSettingsWidget::on_rbNormal_toggled(bool checked)
{
    if (checked) {
        ui->txtPkcs12Path->setEnabled(false);
        ui->cmdGetPkcs12Path->setEnabled(false);
        // Die normalen deaktivieren
        ui->txtKey->setEnabled(true);
        ui->txtCA->setEnabled(true);
        ui->txtCert->setEnabled(true);

        ui->cbCertIsServer->setEnabled(true);
        ui->cmdGetCAPath->setEnabled(true);
        ui->cmdGetCertPath->setEnabled(true);
        ui->cmdGetKeyPath->setEnabled(true);
    }
}

void AdvConfigSettingsWidget::on_cmdAdd_clicked()
{
    if(ui->txtRemoteIP->text().isEmpty())
    {
        return;
    }

    QList<QStandardItem*> newRow;
    QStandardItemModel * m = (QStandardItemModel*)ui->lvRemote->model();
    QStandardItem* itm = new QStandardItem(ui->txtRemoteIP->text());
    newRow.append(itm);
    itm = new QStandardItem(ui->txtRemotePort->text());
    newRow.append(itm);

    m->appendRow(newRow);

    ui->txtRemotePort->setText("");
    ui->txtRemoteIP->setText("");
}

void AdvConfigSettingsWidget::on_cmdRemove_clicked()
{
    QModelIndexList indexes = this->ui->lvRemote->selectionModel()->selectedRows();
    while (!indexes.isEmpty())
    {
        (QStandardItemModel*)this->ui->lvRemote->model()->removeRows(indexes.last().row(), 1);
        indexes.removeLast();
    }
}
