#include "preferences.h"
#include "ui_preferences.h"
#include "mainwindowcontroll.h"

// The most classes used here are singletons.

Preferences::Preferences(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::Preferences)
{
    m_ui->setupUi(this);

    // Kontrollieren ob in der Portable Version
    // das TAP Device installiert ist.
    if (Settings::getInstance()->getIsPortableClient()) {
        #ifdef Q_OS_WIN32
           // Windows
           if (!TapDriver::getInstance()->isTapDriverInstalled()) {
               QMessageBox msgBox;
                           msgBox.setWindowTitle(tr("Securepoint SSL VPN"));
                           msgBox.setWindowIcon(QIcon(":/images/logo.png"));
                           msgBox.setText(tr("No Tap-Win32 driver was found on this system."));
                           msgBox.setInformativeText(tr("Install the driver?\nFor this action you need administrator permissions!"));
                           msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
                           msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
                           msgBox.setDefaultButton(QMessageBox::Yes);
               int ret = msgBox.exec();
               switch (ret) {
                   case QMessageBox::Yes:
                        if (!TapDriver::getInstance()->installTapDriver()) {
                           QMessageBox msgBox;
                           msgBox.setWindowTitle(tr("Securepoint SSL VPN"));
                           msgBox.setText(tr("Install Tap Device"));
                           msgBox.setWindowIcon(QIcon(":/images/logo.png"));
                           msgBox.setInformativeText(QString(tr("Unable to install Tap-Win32 driver!\nMaybe you have no permissions.\nPlease contact your system administrator.")));
                           msgBox.setStandardButtons(QMessageBox::Ok);
                           msgBox.setDefaultButton(QMessageBox::Ok);
                           msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
                           msgBox.exec();
                           QCoreApplication::exit(1);
                       }
                       break;
                   case QMessageBox::Cancel:
                   default:
                       QCoreApplication::exit(1);
                       break;
               }
           }
        #elif Q_OS_Unix
             // Linux
             // I hope next release
        #elif Q_OS_Mac
           // mac
        #endif
    }
    // Ende Tap Install
    m_ui->extensionWidget->setVisible(false);
    this->setMinimumHeight(390);
    this->setMaximumHeight(390);

    this->setWindowFlags(Qt::MSWindowsFixedSizeDialogHint | Qt::WindowCloseButtonHint);

    // Icon setzen und anzeigen
    this->configPwd = "";
    this->configUser = "";

    this->trayIconMenu = NULL;

    // Set window title
    QString titleAdd = "";
    if (Settings::getInstance()->getIsManageClient())
        titleAdd = tr (" - manage");

    if (Settings::getInstance()->getIsPortableClient())
        titleAdd = tr(" - portable");

    setWindowTitle(tr("Securepoint SSL VPN") + titleAdd);
}

void Preferences::searchStartConfigDir(){
    this->refreshConfigList();
    createActions();
    createTrayIcon();
    QIcon icon = QIcon(":/images/inaktiv.png");
    trayIcon->setIcon(icon);

    trayIcon->show();

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
             this, SLOT(trayActivated(QSystemTrayIcon::ActivationReason)));
    this->refreshDialog();
}

bool Preferences::isConnectionActive() const {
    bool retVal = false;
    foreach (OpenVpn* obj, Configs::getInstance()->getConfigsObjects()) {
        if (obj->isConnectionStable()) {
            retVal = true;
        } else if (obj->isConnecting()) {
            retVal = true;
        }
    }
    return retVal;
}

void Preferences::trayActivated(QSystemTrayIcon::ActivationReason reason){
    if (reason == QSystemTrayIcon::DoubleClick)
        if (!this->isMinimized()) {
            this->refreshDialog();
            if (Settings::getInstance()->getIsPortableClient() || Settings::getInstance()->getIsManageClient()) {
                m_ui->cmdNewConfig->show();
                m_ui->cmdImportConfig->show();
                m_ui->cmdRefreshData->show();
            } else {
                m_ui->cmdNewConfig->hide();
                m_ui->cmdImportConfig->hide();
                m_ui->cmdRefreshData->hide();
            }
            this->showNormal();
            this->setFocus();
            this->activateWindow();
        }
}

void Preferences::refreshConfigList() {
    // Listen löschen
    Configs::getInstance()->clearConfigs();
    // Liste neu einlesen
    Configs::getInstance()->searchConfigs(AppFunc::getInstance()->getAppSavePath());

    if (Settings::getInstance()->getIsStartConfigDir())
        Configs::getInstance()->searchConfigs(Settings::getInstance()->getStartConfigDirPath());

    //Configs::getInstance()->searchConfigs(QDir::currentPath());
    Configs::getInstance()->searchLinkedConfigs();    
}

void Preferences::setConnectionStatus() {
    // TrayIcon neu aufbauen
    trayIcon->hide();
    createActions();
    createTrayIcon();
    QIcon icon = QIcon(":/images/inaktiv.png");
    trayIcon->setIcon(icon);
    trayIcon->show();

    // Das noch verbundene Element setzen
    foreach (OpenVpn* obj, Configs::getInstance()->getConfigsObjects()) {
        if (obj->isConnectionStable()) {
            obj->disableMenues();
            QIcon iconCon = QIcon(":/images/connected.png");
            MainWindowControll::getInstance()->setTrayIcon(iconCon);
            break;
        } else if (obj->isConnecting()) {
            obj->disableMenues();
            QIcon iconConnecting = QIcon(":/images/connecting.png");
            MainWindowControll::getInstance()->setTrayIcon(iconConnecting);
            break;
        }
    }
}

void Preferences::refreshDialog() {    
    if (!Settings::getInstance()->getIsRunAsSevice())
        MainWindowControll::getInstance()->setMainWindow(this);
    // Neue Version
    m_ui->trvConnections->clear();
    QStringList  header;
    header << "ID" << "Name" << "Action" << "State" << "Stateid";
    m_ui->trvConnections->setColumnCount(5);
    m_ui->trvConnections->setHeaderLabels(header);
    m_ui->trvConnections->header()->setMovable(false);
    m_ui->trvConnections->header()->hideSection(0);
    m_ui->trvConnections->header()->hideSection(4);
    m_ui->trvConnections->header()->hide();
    // Neue Version Ende

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
             this, SLOT(trayActivated(QSystemTrayIcon::ActivationReason)));
    foreach (OpenVpn *configObj, Configs::getInstance()->getConfigsObjects()) {
        disconnect(configObj, SIGNAL(errorOccuredSig(QString)), 0, 0);
        disconnect(configObj, SIGNAL(connectionIsStableSig(QString)), 0, 0);
        disconnect(configObj, SIGNAL(connectionIsDisconnectedSig()), 0, 0);
        // Neue Version
        TreeConItem *item = new TreeConItem (m_ui->trvConnections->invisibleRootItem());

        // OpenVpn Obj merken
        item->setOpenVPN(configObj);

        // Namen setzen
        QString linkedText = "";
        if (configObj->isLinked)
            linkedText = " - ";
        item->setText(1, linkedText + configObj->configName + configObj->getAdvName());
        item->setToolTip(1, configObj->configPath.replace("/", "\\") + QString("\\") + configObj->configName + QString (".ovpn"));
        // Grundstatus setzen
        if (Settings::getInstance()->getUseCryptedData() && configObj->getIsCryptFileAvailable()) {
            item->setIcon(2, QIcon(":/images/crypted.png"));
        }
        // Neune Button für die Verbindung generieren
        TreeButton *button = new TreeButton ();

        button->setStyleSheet(QString("border: 2px solid black; background-color: white; height: 30px; width: 80px; margin-right: 10px;"));
        button->setFlat(true);
        // Item zum Button merken zwecks sender
        button->setParentItem(item);        
        // Signale des Objektes an den Button binden
        connect (configObj, SIGNAL(errorOccuredSig(QString)), button, SLOT(receivedError(QString)));
        connect (configObj, SIGNAL(connectionIsStableSig(QString)), button, SLOT(receivedConnctionIsStable(QString)));
        connect (configObj, SIGNAL(connectionIsDisconnectedSig()), button, SLOT(receivedDisconnect()));
        connect (configObj, SIGNAL(connectionReconnectFromOpenVPNSig()), button, SLOT(receivedConnecting()));
        // Objekt auswerten
        if (configObj->isConnectionStable()) {
            if (Settings::getInstance()->getIsRunAsSevice()) {
                this->data->setTreeButton(button);
            }
            button->setConnected(configObj->getConnectionIP());
        } else {
            if (configObj->isConnecting()) {
                if (Settings::getInstance()->getIsRunAsSevice()) {
                    this->data->setTreeButton(button);
                }
                button->setConnecting();
            } else {
                if (configObj->isError()) {
                    button->setError(configObj->getErrorString());
                } else {
                    button->setReadyToConnect();
                }
            }
        }
        connect (button, SIGNAL(clicked()), this, SLOT(treePushButtonClicked()));
        m_ui->trvConnections->setItemWidget(item, 3, button);
        // Item merkt sich auch den Button
        item->setTreeButton(button);

        m_ui->trvConnections->addTopLevelItem(item);
        // Neue Version Ende
    }
    // Neue Version
    m_ui->trvConnections->resizeColumnToContents(1);
    m_ui->trvConnections->header()->resizeSection(2, 170);
    m_ui->trvConnections->header()->resizeSection(1, 130);
    // Neue Versioen Ende

    // Statusfelder updaten
    m_ui->lblConnectVia->setText((Settings::getInstance()->getIsRunAsSevice() ? tr("Service") : tr("OpenVPN")));
    m_ui->lblReconnect->setText((Settings::getInstance()->getIsAutoReconnect() ? tr("yes") : tr("no")));
    m_ui->lblShowBallon->setText((Settings::getInstance()->getIsShowNoBallonMessage() ? tr("no") : tr("yes") ));
    m_ui->lblShowAuth->setText((Settings::getInstance()->getIsShowNoUserAuth() ? tr("no") : tr("yes")));
    m_ui->lblStartUser->setText((Settings::getInstance()->getIsStartWithUser() ? tr("yes") : tr("no")));
    m_ui->lblStartPwd->setText((Settings::getInstance()->getIsStartWithPwd() ? tr("yes") : tr("no")));
    m_ui->lblSearchDir->setText((Settings::getInstance()->getIsStartConfigDir() ? Settings::getInstance()->getStartConfigDirPath() : tr("none")));
    m_ui->lblNoPopUp->setText((Settings::getInstance()->getIsNoPopUp() ? tr("yes") : tr ("no")));
    m_ui->lblStartConfig->setText((Settings::getInstance()->getIsStartCommandConfig() ? tr("yes") : tr("no")));
    m_ui->lblUserCrypt->setText((Settings::getInstance()->getUseCryptedData() ? tr ("yes") : tr ("no")));
    if (Settings::getInstance()->getUseCryptedData()) {
        m_ui->cbForcePrivKey->setEnabled(true);
    } else {
        m_ui->cbForcePrivKey->setEnabled(false);
    }
    m_ui->cbForcePrivKey->setChecked(Settings::getInstance()->getIsForcePrivateKey());
    m_ui->lblLanguage->setText((Settings::getInstance()->getIsLanguageGerman() ? tr ("German") : tr ("English")));
}

void Preferences::setVisible(bool visible)
{
    preferencesAction->setEnabled(isMaximized() || !visible);
    QDialog::setVisible(visible);
}

Preferences::~Preferences()
{
    delete m_ui;
}

void Preferences::startDaemon(){
    MainWindowControll::getInstance()->setMainWindow(this);
    data = new DataControll(Settings::getInstance()->getListenPort(), this);
    data->servicePort = Settings::getInstance()->getServerPort();
}

void Preferences::openDialog(bool configFromCommandLine, QString commandLineConfig) {
    bool showMe (false);
    if (configFromCommandLine) {

        QString commandConfigDir = "";
        QString commandConfigName = "";

        // OpenVPN Objekt aus der übergebenen Config erzeugen
        // Name und Pfad ermitteln
        commandConfigName = commandLineConfig.right(commandLineConfig.size() - commandLineConfig.lastIndexOf("\\") -1);
        commandConfigName = commandConfigName.left(commandConfigName.size()-5);
        // Pfad
        commandConfigDir = commandLineConfig.left(commandLineConfig.lastIndexOf("\\"));

        // Zuerst schauen, ob die Config schon in der Liste ist.
        TreeConItem *startItem = NULL;
        int x;
        for (x=0;x < m_ui->trvConnections->invisibleRootItem()->childCount();x++) {
            // Init
            bool dirIsEqual = false;
            bool nameIsEqual = false;

            TreeConItem *item = dynamic_cast<TreeConItem*>(m_ui->trvConnections->invisibleRootItem()->child(x));
            // Get OpenVpn Obj
            QString configDirFormat = item->getOpenVPN()->configPath.replace("\\", "/").trimmed().toLower();
            QString commandDirFormat = commandConfigDir.replace("\\", "/").trimmed().toLower();
            // Ist das Verzeichnis gleich?
            if (configDirFormat == commandDirFormat)
                dirIsEqual = true;
            QString configNameFormat = item->getOpenVPN()->configName.trimmed().toLower();
            QString commandNameFormat = commandConfigName.trimmed().toLower();
            // Ist der Name gleich
            if (configNameFormat == commandNameFormat)
                nameIsEqual = true;

            // Config gefunden
            if (dirIsEqual && nameIsEqual) {
                startItem = item;
                break;
            }
        }
        // Ist Objekt dabei, dann starten
        // Andernfalls ein neues erstellen
        if (startItem) {
            startItem->getOpenVPN()->setAdvName(QString ("\n") + QString (tr("- Start config")));
            startItem->setText(1, startItem->text(1) + QString ("\n") + QString (tr("- Start config")));
            startItem->getTreeButton()->click();           
        } else {
            // Angegebene Config ist noch nicht vorhanden
            // Objekt erzeugen
            OpenVpn* myObj = new OpenVpn();
            myObj->connectionStable = false;
            myObj->configName = commandConfigName;
            myObj->configPath = commandConfigDir;
            myObj->setAdvName(QString ("\n") + QString (tr("- Start config")));

            // Objekt der Liste anhängen
            Configs::getInstance()->appendConfigToList(myObj);
            // Objekt da, nun muss noch der Button und das Item erzeugt werden
            TreeConItem *item = new TreeConItem (m_ui->trvConnections->invisibleRootItem());

            // OpenVpn Obj merken
            item->setOpenVPN(myObj);

            // Namen setzen
            item->setText(1, myObj->configName + QString ("\n") + QString (tr("- Start config")));
            item->setToolTip(1, myObj->configPath.replace("/", "\\") + QString ("\\") + myObj->configName + QString (".ovpn"));
            // Grundstatus setzen

            // Neune Button für die Verbindung generieren
            TreeButton *button = new TreeButton ();

            button->setStyleSheet(QString("border: 2px solid black; background-color: white;  height: 30px; width: 80px; margin-right: 10px;"));
            button->setFlat(true);
            // Item zum Button merken zwecks sender
            button->setParentItem(item);            
            // Signale des Objektes an den Button binden
            connect (myObj, SIGNAL(errorOccuredSig(QString)), button, SLOT(receivedError(QString)));
            connect (myObj, SIGNAL(connectionIsStableSig(QString)), button, SLOT(receivedConnctionIsStable(QString)));
            connect (myObj, SIGNAL(connectionIsDisconnectedSig()), button, SLOT(receivedDisconnect()));
            // Button CLick binden
            connect (button, SIGNAL(clicked()), this, SLOT(treePushButtonClicked()));
            // Button in den Tree einfügen
            m_ui->trvConnections->setItemWidget(item, 3, button);
            // Item merkt sich auch den Button
            item->setTreeButton(button);
            // Item anzeigen
            m_ui->trvConnections->addTopLevelItem(item);
            // Objekt ist nicht verbunden
            button->setReadyToConnect();
            // Alles erzeugt, Click auslösen
            button->click();
        }
        // Den Dialog bei Startconfig immer anzeigen.
        if (!Settings::getInstance()->getIsNoPopUp())
            showMe = true;
    } else {
        this->refreshDialog();
        showMe = true;
    }

    if (Settings::getInstance()->getIsPortableClient() || Settings::getInstance()->getIsManageClient()) {
        m_ui->cmdNewConfig->show();
        m_ui->cmdImportConfig->show();
        m_ui->cmdRefreshData->show();
    } else {
        m_ui->cmdNewConfig->hide();
        m_ui->cmdImportConfig->hide();
        m_ui->cmdRefreshData->hide();
    }

    if (showMe)
        this->show();

}

void Preferences::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible()) {
        hide();
        event->ignore();
    }
}

void Preferences::openInfo() {
    AppInfo::getInstance()->show();
}

void Preferences::on_cmdClose_clicked()
{
    this->close();
}

void Preferences::manageConnections () {
    this->openDialog(false, QString (""));
}

void Preferences::createTrayIcon()
{
    // Menü bauen
    if (this->trayIconMenu != NULL) {
        delete this->trayIconMenu;
        this->trayIconMenu = NULL;
    }
    trayIconMenu = new QMenu(this);
    trayIconMenu->setObjectName("SYSTRAYMENU");
    if (Settings::getInstance()->getIsPortableClient()) {
        foreach (OpenVpn *configObj, Configs::getInstance()->getConfigsObjects()) {            
            // Make new Menu from Config
            QMenu *trayTest = new QMenu(configObj->configName, this);
            // Set Child of Menu
            QAction *mySubAction = new QAction(QIcon(":/images/connectmen.png"), tr ("Connect"), this);
            mySubAction->setObjectName("CONNECT");
            connect(mySubAction, SIGNAL(triggered()), configObj, SLOT(openConnect()));
            trayTest->addAction (mySubAction);

            mySubAction = new QAction(QIcon(":/images/disconnetdmen.png"), tr ("Disconnect"), this);
            mySubAction->setObjectName("DISCONNECT");
            mySubAction->setDisabled(true);
            connect(mySubAction, SIGNAL(triggered()), configObj, SLOT(disconnectVpn()));
            trayTest->addAction (mySubAction);

            trayTest->addSeparator();

            mySubAction = new QAction(QIcon(":/images/logstartstop.png"), tr ("Show log"), this);
            mySubAction->setObjectName(configObj->configName);
            connect(mySubAction, SIGNAL(triggered()), configObj, SLOT(openVpnLog()));
            trayTest->addAction (mySubAction);

            mySubAction = new QAction(QIcon(":/images/edit.png"), tr ("Quick edit"), this);
            mySubAction->setObjectName(configObj->configName);
            connect(mySubAction, SIGNAL(triggered()), configObj, SLOT(openEditConfig()));
            trayTest->addAction (mySubAction);
            // Append to Main Menu
            configObj->menu = trayIconMenu->addMenu(trayTest);
        }
        trayIconMenu->addSeparator();
    }

    trayIconMenu->addAction(preferencesAction);
    trayIconMenu->addSeparator();
    if (Settings::getInstance()->getIsPortableClient() || Settings::getInstance()->getIsManageClient()) {
        trayIconMenu->addAction(proxyAction);
        trayIconMenu->addAction(appInfoAction);
        trayIconMenu->addSeparator();
    }
    trayIconMenu->addAction(quitAction);
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
}

void Preferences::createActions()
{
    // Normale Actions
    preferencesAction = new QAction(QIcon(":/images/status.png"),tr("&Status"), this);
    preferencesAction->setEnabled(!this->isVisible());
    connect(preferencesAction, SIGNAL(triggered()), this, SLOT(manageConnections()));

    if (Settings::getInstance()->getIsPortableClient() || Settings::getInstance()->getIsManageClient()) {
        proxyAction = new QAction (QIcon(":/images/proxy.png"),tr("&Proxy Settings"), this);
        connect(proxyAction, SIGNAL(triggered()), this, SLOT(openProxySettings()));

        appInfoAction = new QAction (QIcon(":/images/info.png"),tr("&Information"), this);
        connect (appInfoAction, SIGNAL(triggered()), this, SLOT(openAppInfo()));
    }

    quitAction = new QAction(QIcon(":/images/system-log-out.png"),tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), this, SLOT(closeApp()));
}

void Preferences::closeApp() {
    // Prüfen, ob noch eine Verbindung aktiv ist
    if (Settings::getInstance()->getIsRunAsSevice()) {
        if (data->isConnectedService) {
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("Securepoint SSL VPN"));
            msgBox.setText(tr("A connection is still connected!"));
            msgBox.setWindowIcon(QIcon(":/images/logo.png"));
            msgBox.setInformativeText(tr("You are still connected. Please disconnect first."));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
            msgBox.exec();
            return;
        }
    } else {
        foreach (OpenVpn *configObj, Configs::getInstance()->getConfigsObjects()) {
            if (configObj->isConnectionStable() || configObj->isOnConnect()) {
                QMessageBox msgBox;
                msgBox.setWindowTitle(tr("Securepoint SSL VPN"));
                msgBox.setText(tr("A connection is still connected!"));
                msgBox.setWindowIcon(QIcon(":/images/logo.png"));
                msgBox.setInformativeText(tr("Do you want to disconnect the connection?"));
                msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                msgBox.setDefaultButton(QMessageBox::Yes);
                msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
                int ret = msgBox.exec();
                switch (ret) {
                    case QMessageBox::Yes:
                         configObj->disconnectVpn();
                         break;
                     case QMessageBox::No:
                     default:
                         break;
                }
            }
        }
    }
    if (Settings::getInstance()->getIsPortableClient()) {
#ifdef Q_OS_WIN32
        if (TapDriver::getInstance()->isTapDriverInstalled()) {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Securepoint SSL VPN");
            msgBox.setText(tr("Remove Tap-Win32 driver"));
            msgBox.setWindowIcon(QIcon(":/images/logo.png"));
            msgBox.setInformativeText(tr("Uninstall the driver?\nFor this action you need administrator permissions!"));
            msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::Yes);
            int ret = msgBox.exec();
            switch (ret) {
            case QMessageBox::Yes:
                if (!TapDriver::getInstance()->removeTapDriver()) {
                    QMessageBox msgBox;
                    msgBox.setWindowTitle(tr("Securepoint SSL VPN"));
                    msgBox.setText(tr("Remove Tap-Win32 driver"));
                    msgBox.setWindowIcon(QIcon(":/images/logo.png"));
                    msgBox.setInformativeText(QString(tr("Unable to uninstall Tap-Win32 driver!\nMaybe you have no permissions.\nPlease contact your system administrator.")));
                    msgBox.setStandardButtons(QMessageBox::Ok);
                    msgBox.setDefaultButton(QMessageBox::Ok);
                    msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
                    msgBox.exec();
                    QCoreApplication::exit(1);
                }
                break;
                         case QMessageBox::No:
                         default:
                QCoreApplication::exit(1);
                break;
            }
        }
#endif
    }
    QCoreApplication::exit(0);
}

void Preferences::setIcon(int index) {
    QIcon icon;
    switch (index) {
        case Inaktiv:
            icon = QIcon(":/images/inaktiv.png");
            break;
        case Connected:
            icon = QIcon(":/images/connected.png");
            break;
        case Error:
            icon = QIcon(":/images/disconnected.png");
            break;
        case Connecting:
            icon = QIcon(":/images/connecting.png");
            break;
        default:
            icon = QIcon(":/images/inaktiv.png");
            break;
    }
    this->trayIcon->setIcon(icon);
}

void Preferences::showTrayMessage(QString message, QSystemTrayIcon::MessageIcon MessageType) {
    this->trayIcon->showMessage(tr("Securepoint SSL VPN"), message, MessageType, 3000);
}

void Preferences::startDelayDisconnectScript() {
    data->runScript("AD");
}

void Preferences::on_cmdOpenInfo_clicked()
{
    AppInfo::getInstance()->show();
}

void Preferences::treePushButtonClicked() {
    TreeButton *but = dynamic_cast<TreeButton*>(sender ());
    // Wenn über Dienst die Daten geholt werden, den Treebutton in
    // der DataControll setzen, um die GUI ansprechen zu können
    int x=0;
    bool someOnline=false;
    bool isSame = false;
    int state = 0;
    for (x=0;x < m_ui->trvConnections->invisibleRootItem()->childCount();x++) {
        TreeConItem *item = dynamic_cast<TreeConItem*>(m_ui->trvConnections->invisibleRootItem()->child(x));
        // Connected oder connecting
        if (item->text(4) == "1" || item->text(4) == "2") {
            if (item->getTreeButton() != but) {
                someOnline = true;                
                break;
            } else {
                state = item->text(4).toInt();
                isSame = true;
            }
        }
    }

    if (!someOnline) {
        if (isSame) {
            if (state == 2) {
                // Make Disconnect
                QMessageBox msgBox;
                            msgBox.setWindowTitle("Securepoint SSL VPN");
                            msgBox.setWindowIcon(QIcon(":/images/logo.png"));
                            msgBox.setText(tr("Disconnect"));
                            msgBox.setInformativeText(tr("Do you want to disconnect the connection?"));
                            msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
                            msgBox.setDefaultButton(QMessageBox::Ok);
                            msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
                int ret = msgBox.exec();
                switch (ret) {
                case QMessageBox::Ok: {
                        if (Settings::getInstance()->getIsRunAsSevice()) {
                            this->data->setTreeButton(but);
                            this->data->sendDisconnect();
                        } else {
                            but->getParentItemCast()->getOpenVPN()->disconnectVpn();
                        }
                    }
                    case QMessageBox::Cancel:
                    default:
                        break;
                }
            } else if (state == 1) {
                if (Settings::getInstance()->getIsRunAsSevice()) {
                    this->data->setTreeButton(but);
                    this->data->sendDisconnect();
                } else {
                    but->getParentItemCast()->getOpenVPN()->disconnectVpn();
                }
            } else {
                but->setReadyToConnect();
            }
            this->enableTreeButtons();
        } else {
            this->disableTreeButtons(but);
            but->setConnecting();
            if (Settings::getInstance()->getIsRunAsSevice()) {
                this->data->setTreeButton(but);
                // Start with service
                // Now Connect new config
                if (!data->isConnectedService) {
                    data->sendDisconnectWithoutGUI();
                    but->setConnecting();
                    data->config = but->getParentItemCast()->getOpenVPN()->configPath + "/" + but->getParentItemCast()->getOpenVPN()->configName + ".ovpn";
                    if (Settings::getInstance()->getIsShowNoUserAuth()) {
                        if (Settings::getInstance()->getIsStartWithUser())
                            data->configUser = Settings::getInstance()->getStartWithUser();

                        if (Settings::getInstance()->getIsStartWithPwd())
                            data->configPwd = Settings::getInstance()->getStartWithPwdPwd();
                        but->setConnecting();
                        data->sendConnect();
                    } else {
                        but->setConnecting();
                        data->sendConnect();
                    }
                } else {
                    QMessageBox msgBox;
                    msgBox.setWindowTitle(tr("Securepoint SSL VPN"));
                    msgBox.setText(tr("A connection is still connected!"));
                    msgBox.setWindowIcon(QIcon(":/images/logo.png"));
                    msgBox.setInformativeText(tr("You are still connected. Please disconnect first."));
                    msgBox.setStandardButtons(QMessageBox::Ok);
                    msgBox.setDefaultButton(QMessageBox::Ok);
                    msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
                    msgBox.exec();
                    return;
                }
            } else {
                if (!but->getParentItemCast()->getOpenVPN()->isConnectionStable()) {
                    but->getParentItemCast()->getOpenVPN()->openConnect();
                    //this->close();
                }
            }
        }
    } else {
        but->setError(QString (tr("You are already connected.")));
    }
}

void Preferences::disableTreeButtons(TreeButton *button) {
    int x=0;
    for (x=0;x < m_ui->trvConnections->invisibleRootItem()->childCount();x++) {
        TreeConItem *item = dynamic_cast<TreeConItem*>(m_ui->trvConnections->invisibleRootItem()->child(x));
        if (item->getTreeButton() != button)
            item->getTreeButton()->setEnabled(false);
    }
}

void Preferences::enableTreeButtons() {
    int x=0;
    for (x=0;x < m_ui->trvConnections->invisibleRootItem()->childCount();x++) {
        TreeConItem *item = dynamic_cast<TreeConItem*>(m_ui->trvConnections->invisibleRootItem()->child(x));
        item->getTreeButton()->setEnabled(true);
    }
}



void Preferences::on_trvConnections_itemDoubleClicked(QTreeWidgetItem* item, int column)
{
    if (column == 1) {
        if (Settings::getInstance()->getIsManageClient()) {
            TreeConItem* _item(dynamic_cast<TreeConItem*>(item));
            if (_item) {
                ManageConnection::getInstance()->setOpenVpnObject(_item->getOpenVPN());
                ManageConnection::getInstance()->show();
            }
        }
    } else if (column == 2) {
        if (Settings::getInstance()->getIsRunAsSevice()) {
            ServiceLog::getInstance()->show();
        } else {
            TreeConItem *_item = dynamic_cast<TreeConItem*>(item);
            if (_item) {
                _item->getOpenVPN()->openVpnLog();
            }
        }
    }
}

void Preferences::on_cmdToggleExtensions_clicked()
{
    if (!m_ui->extensionWidget->isVisible()) {
        m_ui->extensionWidget->setVisible(true);
        this->setMaximumHeight(550);
        this->setMinimumHeight(550);
    } else {
        m_ui->extensionWidget->setVisible(false);
        this->setMinimumHeight(390);
        this->setMaximumHeight(390);
    }
}

void Preferences::on_cbForcePrivKey_toggled(bool checked)
{
    Settings::getInstance()->setForcePrivateKey(checked);
}

void Preferences::openProxySettings() {
    ProxySettings::getInstance()->show();
}
void Preferences::openAppInfo() {
    AppInfo::getInstance()->show();
}

void Preferences::on_cmdNewConfig_clicked()
{
    // Nun den Wizard aufrufen
    VpnWizard::getInstance()->show();
}

void Preferences::showLogFromContextMenu() {
    if (Settings::getInstance()->getIsRunAsSevice()) {
        ServiceLog::getInstance()->show();
    }
}

void Preferences::on_trvConnections_customContextMenuRequested(QPoint pos)
{
    // Nur anzeigen wenn Manage oder Portable ist
    if (Settings::getInstance()->getIsPortableClient() || Settings::getInstance()->getIsManageClient()) {
        TreeConItem *item = NULL;
        item = dynamic_cast<TreeConItem*>(m_ui->trvConnections->itemAt(pos));
        if (item) {
            // Dialoge inititiieren
            DeleteConfig::getInstance()->setOpenVpnObject(item->getOpenVPN());
            RenameConfig::getInstance()->setOldName(item->getOpenVPN()->configName);
            RenameConfig::getInstance()->setFullPath(item->getOpenVPN()->configPath + "/" + item->getOpenVPN()->configName + ".ovpn");
            ManageConnection::getInstance()->setOpenVpnObject(item->getOpenVPN());


            QMenu *configPopUp = new QMenu (this);
            bool toggleMenues = true;
            // Connect
            if (item->getOpenVPN()->isConnecting() || item->getOpenVPN()->isConnectionStable()) {
                configPopUp->addAction(QPixmap(":/images/disconnetdmen.png"), tr("&Disconnect"), item->getTreeButton(), SLOT(click()));
            } else {
                configPopUp->addAction(QPixmap(":/images/connectmen.png"), tr("Con&nect"), item->getTreeButton(), SLOT(click()));
            }
            if (Settings::getInstance()->getIsRunAsSevice()) {
                configPopUp->addSeparator();
                configPopUp->addAction(QPixmap(":/images/logstartstop.png"), tr("Service log"), this, SLOT(showLogFromContextMenu()));
            }
            configPopUp->addSeparator();
            // Delete
            // Wenn die Verbindung hergestellt ist oder gerade beim connecten
            // den Menupunkt deaktivieren
            if (item->getOpenVPN()->isConnecting() || item->getOpenVPN()->isConnectionStable())
                toggleMenues = false;
            if (item->getOpenVPN()->isLinked) {
                configPopUp->addAction(QPixmap(":/images/delete.png"), tr("&Delete"), this, SLOT(deleteLinkedConfig()))->setEnabled(toggleMenues);
            } else {
                configPopUp->addAction(QPixmap(":/images/delete.png"), tr("&Delete"), DeleteConfig::getInstance(), SLOT(show()))->setEnabled(toggleMenues);
            }
            if (Settings::getInstance()->getUseCryptedData() && item->getOpenVPN()->getIsCryptFileAvailable()) {
                configPopUp->addSeparator();
                configPopUp->addAction(QPixmap(":/images/crypted.png"), tr("&Delete crypted data"), this, SLOT(deleteCryptedData()))->setEnabled(toggleMenues);
            }
            configPopUp->addSeparator();
            configPopUp->addAction(QPixmap(":/images/rename.png"), tr("&Rename"), RenameConfig::getInstance(), SLOT(show()))->setEnabled(toggleMenues);
            configPopUp->addSeparator();
            configPopUp->addAction(QPixmap(":/images/manage.png"), tr("&Edit"), ManageConnection::getInstance(), SLOT(show()))->setEnabled(toggleMenues);
            configPopUp->addAction(QPixmap(":/images/edit.png"), tr("&Quick edit"), item->getOpenVPN(), SLOT(openEditConfig()))->setEnabled(toggleMenues);
            configPopUp->addSeparator();
            configPopUp->addAction(QPixmap(":/images/export.png"), tr("&Export"), item->getOpenVPN(), SLOT(openExport()));
            //configPopUp->addSeparator();
            configPopUp->addAction(QPixmap(":/images/import.png"), tr("&Import"), ImportConfig::getInstance(), SLOT(show()));
            configPopUp->addSeparator();
            configPopUp->exec(m_ui->trvConnections->mapToGlobal(pos));
        }
    }
}

void Preferences::deleteLinkedConfig() {
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Securepoint SSL VPN"));
    msgBox.setText(tr("Delete linked configuration"));
    msgBox.setWindowIcon(QIcon(":/images/logo.png"));
    msgBox.setInformativeText(tr("Remove the linked configuration from list?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
    int ret = msgBox.exec();
    switch (ret) {
        case QMessageBox::Yes: {
            Configs::getInstance()->removeConfigFromList(DeleteConfig::getInstance()->getOpenVpnObject()->configPath + "/" + DeleteConfig::getInstance()->getOpenVpnObject()->configName + ".ovpn");
            MainWindowControll::getInstance()->refreshConfigs();
            MainWindowControll::getInstance()->setConnectionStatus();
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("Securepoint SSL VPN"));
            msgBox.setText(tr("Delete linked configuration"));
            msgBox.setWindowIcon(QIcon(":/images/logo.png"));
            msgBox.setInformativeText(tr("Configuration successfully removed!"));
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::Yes);
            msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
            msgBox.exec();
            MainWindowControll::getInstance()->refreshDialog();
            break;
        }
        case QMessageBox::No:
        default:
            break;
    }
}

void Preferences::deleteCryptedData() {
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Securepoint SSL VPN"));
    msgBox.setText(tr("Delete crypted user data"));
    msgBox.setWindowIcon(QIcon(":/images/logo.png"));
    msgBox.setInformativeText(tr("Remove the crypted user data for the configuration?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
    int ret = msgBox.exec();
    switch (ret) {
        case QMessageBox::Yes: {
            QFile caFile(DeleteConfig::getInstance()->getOpenVpnObject()->configPath + "/auth.dat");
            if (caFile.exists())
               if (!caFile.remove()) {
                  // Nichts
               }
            MainWindowControll::getInstance()->refreshConfigs();
            MainWindowControll::getInstance()->setConnectionStatus();
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("Securepoint SSL VPN"));
            msgBox.setText(tr("Delete crypted user data"));
            msgBox.setWindowIcon(QIcon(":/images/logo.png"));
            msgBox.setInformativeText(tr("Crypted user data successfully removed!"));
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::Yes);
            msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
            msgBox.exec();
            MainWindowControll::getInstance()->refreshDialog();
            break;
        }
        case QMessageBox::No:
        default:
            break;
    }
}

void Preferences::on_cmdRefreshData_clicked()
{
    this->refreshConfigList();
    this->setConnectionStatus();
    this->refreshDialog();
}

void Preferences::on_cmdImportConfig_clicked()
{
    ImportConfig::getInstance()->show();
}
