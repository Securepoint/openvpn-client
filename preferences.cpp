#include "preferences.h"
#include "ui_preferences.h"
#include "importconfig.h"
#include "servicelog.h"
#include "wizard/wiz_vpnwizard.h"

// Normale Includes
#include "appfunc.h"
#include "configs.h"
#include "openvpn.h"
#include "openvpnqlistitem.h"
#include "settings.h"
#include "treebutton.h"
#include "treeconitem.h"

#include "tapdriver.h"

// Form Includes
#include "appinfo.h"
#include "proxysettings.h"
#include "deleteconfig.h"
#include "renameconfig.h"
#include "frmgetuserdata.h"
#include "frmsaveddata.h"

#include "message.h"

#include "network/sslserver.h"
#include "network/srvcli.h"
#include "debug.h"


Preferences *Preferences::mInst = NULL;

Preferences *Preferences::instance()
{
    if (!mInst) {
        mInst = new Preferences;
    }

    return mInst;
}

Preferences::Preferences() :
    QDialog(),
    m_ui(new Ui::Preferences),
    trayIconMenu (0),
    server (0)
{
    m_ui->setupUi(this);

    // Delay ausblenden
    m_ui->lblDelay->setVisible(false);
    m_ui->cmbDelay->setVisible(false);

    // Kontrollieren ob in der Portable Version
    // das TAP Device installiert ist.
    if (Settings::getInstance()->getIsPortableClient()) {
        #ifdef Q_OS_WIN32
           // Windows
           if (!TapDriver::instance()->isTapDriverInstalled()) {
               if (Message::confirm(QObject::tr("Install the driver?\nFor this action you need administrator permissions!"), QObject::tr("Securepoint SSL VPN"))) {
                   if (!TapDriver::instance()->installTapDriver()) {
                       Message::error(QObject::tr("Unable to install Tap-Win32 driver!\nMaybe you have no permissions.\nPlease contact your system administrator."), QObject::tr("Install Tap Device"));
                       QCoreApplication::exit(1);
                   }
               } else {
                   QCoreApplication::exit(1);
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

    // Startgröße setzen
    this->setMinimumHeight(390);
    this->setMaximumHeight(390);

    this->setWindowFlags(Qt::WindowCloseButtonHint);

    // Set window title
    QString titleAdd (QObject::tr(" - portable"));
    // Ist der Client mit manage aufgerufen?
    if (Settings::getInstance()->getIsManageClient()) {
        titleAdd = QObject::tr(" - manage");
    }

    this->setWindowTitle(QObject::tr("Securepoint SSL VPN RC4") + titleAdd);
}

void Preferences::searchStartConfigDir()
{
    this->refreshConfigList();
    createActions();
    createTrayIcon();    
    trayIcon->show();

    this->refreshDialog();
}

QSystemTrayIcon *Preferences::getSystrayIcon() const
{
    return this->trayIcon;
}

void Preferences::setDisconnected(int id)
{
    // Alle TreeItem durchsuchen bis die ID gefunden wurde
    for (int x = 0; x < m_ui->trvConnections->invisibleRootItem()->childCount(); x++) {
        // Item und OpenVpn Objekt holen
        TreeConItem *item = dynamic_cast<TreeConItem*>(m_ui->trvConnections->invisibleRootItem()->child(x));
        TreeButton *but = item->getTreeButton();
        OpenVpn *vpn = item->getOpenVPN();
        // stimmt die Id
        if (vpn->id() == id) {
            // Objekt gefunden
            // Wieder den normalen Zustand herstellen            
            but->setReadyToConnect();
            vpn->setConfigStable(false);
            vpn->setConnectedIP(QLatin1String(""));
            vpn->setIsConnecting(false);
            vpn->runScript(QLatin1String("AD"));
            break;
        }
    }
    this->setIcon();
}

void Preferences::setError(int id, QString message)
{
    // Alle TreeItem durchsuchen bis die ID gefunden wurde
    for (int x = 0; x < m_ui->trvConnections->invisibleRootItem()->childCount(); x++) {
        // Item und OpenVpn Objekt holen
        TreeConItem *item = dynamic_cast<TreeConItem*>(m_ui->trvConnections->invisibleRootItem()->child(x));
        TreeButton *but = item->getTreeButton();
        OpenVpn *vpn = item->getOpenVPN();
        // stimmt die Id
        if (vpn->id() == id) {
            // Objekt gefunden            
            but->setError(message);
            vpn->setConfigStable(false);
            vpn->setIsError(true);
            vpn->setIsConnecting(false);
            vpn->setConnectedIP(QLatin1String(""));
            vpn->runScript(QLatin1String("EC"));
            this->showTrayMessageChecked(message, QSystemTrayIcon::Critical);
            break;
        }        
    }    
    this->setIcon();
}

void Preferences::receivedIP(int id, QString ip)
{
    // Alle TreeItem durchsuchen bis die ID gefunden wurde
    for (int x = 0; x < m_ui->trvConnections->invisibleRootItem()->childCount(); x++) {
        // Item und OpenVpn Objekt holen
        TreeConItem *item = dynamic_cast<TreeConItem*>(m_ui->trvConnections->invisibleRootItem()->child(x));
        TreeButton *but = item->getTreeButton();
        OpenVpn *vpn = item->getOpenVPN();
        // stimmt die Id
        if (vpn->id() == id) {
            // Objekt gefunden
            but->setConnected(ip);
            vpn->setConnectedIP(ip);
            vpn->setIsConnected(true);
            vpn->setIsConnecting(false);
            vpn->setConnectedSinceDate(QDate::currentDate());
            vpn->setConnectedSinceTime(QTime::currentTime());
            vpn->runDelayStartScript();
            break;
        }
    }
    this->setIcon();
    this->showBallonMessage();
}

void Preferences::receivedReconnect(int id)
{
    // Alle TreeItem durchsuchen bis die ID gefunden wurde
    for (int x = 0; x < m_ui->trvConnections->invisibleRootItem()->childCount(); x++) {
        // Item und OpenVpn Objekt holen
        TreeConItem *item = dynamic_cast<TreeConItem*>(m_ui->trvConnections->invisibleRootItem()->child(x));
        TreeButton *but = item->getTreeButton();
        OpenVpn *vpn = item->getOpenVPN();
        // stimmt die Id
        if (vpn->id() == id) {
            // Objekt gefunden            
            but->setConnecting();
            vpn->setConfigStable(false);
            vpn->setConnectedIP(QLatin1String(""));
            break;
        }
    }
    this->setIcon();
}

void Preferences::receivedTapControl(int type)
{
    // Tap soll installiert werden, wurde erfolgreich installiert oder
    // die Installation ist fehlgeschlagen
    if (type == 0) {
        QApplication::setOverrideCursor(Qt::ArrowCursor);
        Message::information(QObject::tr(("Tap device installed successfully.\nPlease reconnect the connection.")));
    } else if (type == -1) {
        QApplication::setOverrideCursor(Qt::ArrowCursor);
        Message::error(QObject::tr(("Tap device installed failed!")));
    } else if (type == 1) {
        // Neues Device
        if (Message::confirm(QObject::tr("A Tap device ist needed. Do you want to install a device?"))) {
            QApplication::setOverrideCursor(Qt::WaitCursor);
            SrvCLI::instance()->send(QLatin1String("ADDTAP"), QLatin1String(""));
        } else {
            QApplication::setOverrideCursor(Qt::ArrowCursor);
        }
    }
}

void Preferences::showBallonMessage()
{
    // Zeigt eine Ballon-Message beim Trayicon an
    // Dafür werden alle Verbindungen durchlaufen
    QString message ("");
    bool firstConnectedFound (false);
    for (int x = 0; x < m_ui->trvConnections->invisibleRootItem()->childCount(); x++) {
        // Item und OpenVpn Objekt holen
        TreeConItem *item = dynamic_cast<TreeConItem*>(m_ui->trvConnections->invisibleRootItem()->child(x));
        OpenVpn *vpn = item->getOpenVPN();
        if (vpn->isConnectionStable()) {
            if (!firstConnectedFound) {
                message += QObject::tr("Connnected to VPN.\n");
                firstConnectedFound = true;
            }
            message += QObject::tr("Name: ") + vpn->getConfigName() + QLatin1String("\nIP: ") + vpn->getConnectionIP() + QLatin1String("\n");
        }
    }

    this->showTrayMessageChecked(message, QSystemTrayIcon::Information, 6000);
}

bool Preferences::isConnectionActive() const
{
    // Ist eine Verbindung gerade beim verbinden oder schon verbunden?
    // Wird in der single_application im Event-Filter für die WM_QUERYENDSESSION
    // gebraucht
    foreach (ListObject i, Configs::getInstance()->getConfigsObjects()) {
        OpenVpn* obj = i.second;

        if (obj->isConnectionStable()) {
            return true;
        } else if (obj->isConnecting()) {
            return true;
        }
    }

    return false;
}

void Preferences::trayActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::DoubleClick) {
        if (!this->isMinimized()) {
            this->refreshDialog();
            this->setIcon();
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
}

void Preferences::refreshConfigList()
{
    // Listen löschen
    Configs::getInstance()->clearConfigs();
    // Liste neu einlesen
    Configs::getInstance()->searchConfigs(AppFunc::getAppSavePath());

    if (Settings::getInstance()->getIsStartConfigDir())
        Configs::getInstance()->searchConfigs(Settings::getInstance()->getStartConfigDirPath());

    Configs::getInstance()->searchLinkedConfigs();    
}

void Preferences::setConnectionStatus()
{
    // TrayIcon neu aufbauen
    trayIcon->hide();
    createActions();
    createTrayIcon();
    QIcon icon = QIcon(":/images/state/idle.png");
    trayIcon->setIcon(icon);
    trayIcon->show();

    // Das noch verbundene Element setzen
    foreach (ListObject i, Configs::getInstance()->getConfigsObjects()) {
        OpenVpn* obj = i.second;        
        if (obj->isConnectionStable()) {
            obj->disableMenues();                            
        } else if (obj->isConnecting()) {
            obj->disableMenues();                        
        }
    }    
}

void Preferences::refreshDialog()
{            
    m_ui->trvConnections->clear();
    QStringList  header;
    header << QLatin1String("ID") << QLatin1String("Name") << QLatin1String("Action") << QLatin1String("State") << QLatin1String("Stateid");
    m_ui->trvConnections->setColumnCount(5);
    m_ui->trvConnections->setHeaderLabels(header);
    m_ui->trvConnections->header()->setMovable(false);
    m_ui->trvConnections->header()->hideSection(0);
    m_ui->trvConnections->header()->hideSection(4);
    m_ui->trvConnections->header()->hide();

    foreach (ListObject i, Configs::getInstance()->getConfigsObjects()) {
        OpenVpn *configObj = i.second;

        QObject::disconnect(configObj, SIGNAL(errorOccuredSig(QString)), 0, 0);
        QObject::disconnect(configObj, SIGNAL(connectionIsStableSig(QString)), 0, 0);
        QObject::disconnect(configObj, SIGNAL(connectionIsDisconnectedSig()), 0, 0);
        // Neue Version
        TreeConItem *item = new TreeConItem (m_ui->trvConnections->invisibleRootItem());

        // OpenVpn Obj merken
        item->setOpenVPN(configObj);

        // Namen setzen
        QString linkedText ("");
        if (configObj->isConfigLinked()) {
            linkedText = QLatin1String("- ");
        }        
        item->setText(1, linkedText + configObj->getConfigName() + configObj->getAdvName());        
        item->setToolTip(1, configObj->getConfigPath().replace("/", "\\") + QLatin1String("\\") + configObj->getConfigName() + QLatin1String (".ovpn"));
        // Grundstatus setzen        
        if (configObj->hasCrediantials()) {
            item->setIcon(2, QIcon(":/images/crypted.png"));
        }

        // Neune Button für die Verbindung generieren
        TreeButton *button = new TreeButton ();

        button->setStyleSheet(QString("border: 2px solid black; background-color: white; height: 30px; width: 80px; max-height: 30px; max-width: 80px; margin-right: 10px;"));
        button->setFlat(true);
        // Item zum Button merken zwecks sender
        button->setParentItem(item);        
        // Signale des Objektes an den Button binden
        QObject::connect (configObj, SIGNAL(errorOccuredSig(QString)), button, SLOT(receivedError(QString)));
        QObject::connect (configObj, SIGNAL(connectionIsStableSig(QString)), button, SLOT(receivedConnctionIsStable(QString)));
        QObject::connect (configObj, SIGNAL(connectionIsDisconnectedSig()), button, SLOT(receivedDisconnect()));
        QObject::connect (configObj, SIGNAL(connectionReconnectFromOpenVPNSig()), button, SLOT(receivedConnecting()));
        // Objekt auswerten
        if (configObj->isConnectionStable()) {
            button->setConnected(configObj->getConnectionIP());
        } else {
            if (configObj->isConnecting()) {                
                button->setConnecting();
            } else {
                if (configObj->isError()) {                    
                    button->setError(configObj->getErrorString());
                } else {                    
                    button->setReadyToConnect();
                }
            }
        }        
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
    m_ui->lblConnectVia->setText((Settings::getInstance()->getIsRunAsSevice() ? QObject::tr("Service") : QObject::tr("OpenVPN")));
    m_ui->lblReconnect->setText((Settings::getInstance()->getIsAutoReconnect() ? QObject::tr("yes") : QObject::tr("no")));
    m_ui->lblShowBallon->setText((Settings::getInstance()->getIsShowNoBallonMessage() ? QObject::tr("no") : QObject::tr("yes") ));
    m_ui->lblNoPopUp->setText((Settings::getInstance()->popUpDialog() ? QObject::tr("yes") : QObject::tr("no") ));
    m_ui->lblStartupWindows->setText((Settings::getInstance()->autoStartOnWindowsStartup() ? QObject::tr("yes") : QObject::tr("no")));
    m_ui->lblShowSplash->setText((Settings::getInstance()->showSplashScreen() ? QObject::tr("yes") : QObject::tr("no")));
    m_ui->lblSearchDir->setText((Settings::getInstance()->getIsStartConfigDir() ? Settings::getInstance()->getStartConfigDirPath() : QObject::tr("none")));    
    m_ui->lblUseInteract->setText((Settings::getInstance()->getUseNoInteract() ? QObject::tr("yes") : QObject::tr("no") ));

    m_ui->lblLanguage->setText((Settings::getInstance()->getIsLanguageGerman() ? QObject::tr ("German") : QObject::tr ("English")));

    /*
    if (Settings::getInstance()->delayConfig() == QLatin1String("30")) {
        m_ui->cmbDelay->setCurrentIndex(1);
    } else if (Settings::getInstance()->delayConfig() == QLatin1String("60")) {
        m_ui->cmbDelay->setCurrentIndex(2);
    } else if (Settings::getInstance()->delayConfig() == QLatin1String("90")) {
        m_ui->cmbDelay->setCurrentIndex(3);
    } else if (Settings::getInstance()->delayConfig() == QLatin1String("120")) {
        m_ui->cmbDelay->setCurrentIndex(4);
    } else {
        m_ui->cmbDelay->setCurrentIndex(0);
    }
    */

    if (Settings::getInstance()->getIsPortableClient()) {
        m_ui->cmdToogleStartup->setEnabled(false);
    }
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

bool Preferences::startDaemon()
{
    //Debug::setDebugPath(QCoreApplication::applicationDirPath());
    //Debug::enableDebugging(true);
    //Debug::setDebugLevel(DebugLevel::All);
    //Debug::enableMSecs(true);
    this->server = new SslServer (Settings::getInstance()->getListenPort(), this);
    // Disconnect vom Service
    QObject::connect(this->server, SIGNAL(setDisconnected(int)), this, SLOT(setDisconnected(int)));
    // Fehler vom Service
    QObject::connect(this->server, SIGNAL(setError(int,QString)), this, SLOT(setError(int,QString)));
    // User eingabe wird erwartet
    QObject::connect(this->server, SIGNAL(needUserInput(int,int)), this, SLOT(userInputIsNeeded(int,int)));
    // Connection steht
    QObject::connect(this->server, SIGNAL(receivedIP(int,QString)), this, SLOT(receivedIP(int,QString)));
    // Haben einen Reconnect bekommen
    QObject::connect(this->server, SIGNAL(receivedReconnect(int)), this, SLOT(receivedReconnect(int)));
    // Tap Steuerung
    QObject::connect(this->server, SIGNAL(receivedTapControl(int)), this, SLOT(receivedTapControl(int)));
    QObject::connect(this->server, SIGNAL(receivedRemoveTap(QString)), this, SLOT(receivedRemoveTap(QString)));

    // Nun die Verbindung testen
    QTimer timeoutTimer (this);
    timeoutTimer.setSingleShot(true);
    timeoutTimer.setInterval(3000);
    QEventLoop loop;
    QObject::connect(&timeoutTimer, SIGNAL(timeout()), &loop, SLOT(quit()));
    QObject::connect(this->server, SIGNAL(receivedDummy()), &loop, SLOT(quit()));

    SrvCLI::instance()->send(QLatin1String("Dummy"));
    timeoutTimer.start();
    // Blocken
    loop.exec();

    if (timeoutTimer.isActive()) {
        // Alles Ok, Timer kann aus
        timeoutTimer.stop();
    } else {
        // Timeout                    
        return false;
    }

    return true;
}

void Preferences::userInputIsNeeded(int id, int type)
{    
    InputType::UserInputType ntype;
    // 0 - Username
    // 1 - Pwd
    // 2 - OTP
    // 3 - PKCS12
    // 4 - Private Key für Crypted User Data
    QString command;
    switch (type) {
        case 0:
            ntype = InputType::Username;
            command = QLatin1String("UNEEDED");
            break;
        case 1:
            ntype = InputType::Password;
            command = QLatin1String("PWDNEEDED");
            break;
        case 2:
            ntype = InputType::Otp;
            break;
        case 3:
            ntype = InputType::Pkcs12;
            command = QLatin1String("PKNEEDED");
            break;
        case 5:
            ntype = InputType::HttpUsername;
            command = QLatin1String("HTTPUSERNEEDED");
            break;
        case 6:
            ntype = InputType::HttpPassword;
            command = QLatin1String("HTTPPASSNEEDED");
            break;
        default:
            ntype = InputType::Username;
    }

    for (int x = 0; x < m_ui->trvConnections->invisibleRootItem()->childCount(); x++) {
        // Item und OpenVpn Objekt holen
        TreeConItem *item = dynamic_cast<TreeConItem*>(m_ui->trvConnections->invisibleRootItem()->child(x));
        OpenVpn *vpn = item->getOpenVPN();
        // Nun die Daten speichern
        if (vpn->id() == id) {
            if (vpn->hasCrediantials(type)) {                
                // Das ist ein bischen doof hier, geht aber nicht anderes ohne den client neu zu designen
                QString value (vpn->getSavedUserData(type));
                SrvCLI::instance()->send(command, QString("%1;%2").arg(id).arg(value));
                return;
            }
        }
    }

    // Es sind keine Daten gespeichert, neue abfragen
    FrmGetUserData dialog (ntype, id);
    QObject::connect(&dialog, SIGNAL(saveUserData(int,int,QString,bool)), this, SLOT(saveUserData(int,int,QString,bool)));
    dialog.exec();
}

void Preferences::setSavedDataIcon(int id)
{
    for (int x = 0; x < m_ui->trvConnections->invisibleRootItem()->childCount(); x++) {
        // Item und OpenVpn Objekt holen
        TreeConItem *item = dynamic_cast<TreeConItem*>(m_ui->trvConnections->invisibleRootItem()->child(x));
        OpenVpn *vpn = item->getOpenVPN();
        if (vpn->id() == id) {
            item->setIcon(2, QIcon(":/images/crypted.png"));
        }
    }
}

void Preferences::removeSavedDataIcon(int id)
{
    for (int x = 0; x < m_ui->trvConnections->invisibleRootItem()->childCount(); x++) {
        // Item und OpenVpn Objekt holen
        TreeConItem *item = dynamic_cast<TreeConItem*>(m_ui->trvConnections->invisibleRootItem()->child(x));
        OpenVpn *vpn = item->getOpenVPN();
        if (vpn->id() == id) {
            item->setIcon(2, QIcon());
        }
    }
}

void Preferences::saveUserData(int id, int type, QString value, bool save)
{
    for (int x = 0; x < m_ui->trvConnections->invisibleRootItem()->childCount(); x++) {
        // Item und OpenVpn Objekt holen
        TreeConItem *item = dynamic_cast<TreeConItem*>(m_ui->trvConnections->invisibleRootItem()->child(x));
        OpenVpn *vpn = item->getOpenVPN();
        // Nun die Daten speichern
        if (vpn->id() == id) {
            vpn->saveUserData(id, type, value, save);
            return;
        }
    }
}

void Preferences::openDialog(bool configFromCommandLine, QString commandLineConfig)
{
    bool showMe (false);
    if (configFromCommandLine) {
        QString commandConfigDir ("");
        QString commandConfigName ("");

        // OpenVPN Objekt aus der übergebenen Config erzeugen
        // Name und Pfad ermitteln
        commandConfigName = commandLineConfig.right(commandLineConfig.size() - commandLineConfig.lastIndexOf("/") - 1);
        commandConfigName = commandConfigName.left(commandConfigName.size() - 5);
        // Pfad
        commandConfigDir = commandLineConfig.left(commandLineConfig.lastIndexOf("/"));

        // Zuerst schauen, ob die Config schon in der Liste ist.
        TreeConItem *startItem = NULL;
        int x;
        for (x=0;x < m_ui->trvConnections->invisibleRootItem()->childCount();x++) {            
            // Init
            bool dirIsEqual = false;
            bool nameIsEqual = false;

            TreeConItem *item = dynamic_cast<TreeConItem*>(m_ui->trvConnections->invisibleRootItem()->child(x));
            // Get OpenVpn Obj
            QString configDirFormat = item->getOpenVPN()->getConfigPath().replace("\\", "/").trimmed().toLower();
            QString commandDirFormat = commandConfigDir.replace("\\", "/").trimmed().toLower();
            // Ist das Verzeichnis gleich?
            if (configDirFormat == commandDirFormat)
                dirIsEqual = true;
            QString configNameFormat = item->getOpenVPN()->getConfigName().trimmed().toLower();
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
            startItem->getOpenVPN()->setAdvName(QLatin1String ("\n") + QObject::tr("- Start config"));
            startItem->setText(1, startItem->text(1) + QLatin1String ("\n") + QObject::tr("- Start config"));            
            startItem->getTreeButton()->setParentItem(startItem);

            QTimer::singleShot(250, startItem->getTreeButton(), SLOT(click()));

            /*
            if (Settings::getInstance()->delayConfig() == QLatin1String("0")) {
                qDebug() << "no delay";
                QTimer::singleShot(250, startItem->getTreeButton(), SLOT(click()));
            } else {
                int delay (Settings::getInstance()->delayConfig().toInt());
                if (delay <= 0) {
                    // Fehler, auf eine Sekunde setzen
                    delay = 1;
                }
                // Delay in msecs
                delay *= 1000;
                qDebug() << "delay " << delay;
                startItem->setText(2, QObject::tr("Mark to connect with delay"));
                startItem->getOpenVPN()->setDelay(true);
                QTimer::singleShot(delay, startItem->getTreeButton(), SLOT(click()));
            }
            */
        } else {            
            // Angegebene Config ist noch nicht vorhanden
            // Objekt erzeugen
            OpenVpn* myObj = new OpenVpn();
            myObj->setConfigStable(false);
            myObj->setConfigName(commandConfigName);
            myObj->setConfigPath(commandConfigDir);
            myObj->setAdvName(QLatin1String ("\n") + QObject::tr("- Start config"));

            // Objekt der Liste anhängen
            Configs::getInstance()->appendConfigToList(myObj);
            // Objekt da, nun muss noch der Button und das Item erzeugt werden
            TreeConItem *item = new TreeConItem (m_ui->trvConnections->invisibleRootItem());            

            // OpenVpn Obj merken
            item->setOpenVPN(myObj);

            // Namen setzen
            item->setText(1, myObj->getConfigName() + QLatin1String ("\n") + QObject::tr("- Start config"));
            item->setToolTip(1, myObj->getConfigPath().replace("/", "\\") + QLatin1String ("\\") + myObj->getConfigName() + QLatin1String (".ovpn"));
            // Grundstatus setzen

            // Neune Button für die Verbindung generieren
            TreeButton *button = new TreeButton ();

            button->setStyleSheet(QLatin1String("border: 2px solid black; background-color: white;  max-height: 30px; max-width: 80px; height: 30px; width: 80px; margin-right: 10px;"));
            button->setFlat(true);
            // Item zum Button merken zwecks sender
            button->setParentItem(item);            
            // Signale des Objektes an den Button binden
            QObject::connect (myObj, SIGNAL(errorOccuredSig(QString)), button, SLOT(receivedError(QString)));
            QObject::connect (myObj, SIGNAL(connectionIsStableSig(QString)), button, SLOT(receivedConnctionIsStable(QString)));
            QObject::connect (myObj, SIGNAL(connectionIsDisconnectedSig()), button, SLOT(receivedDisconnect()));

            // Button in den Tree einfügen
            m_ui->trvConnections->setItemWidget(item, 3, button);
            // Item merkt sich auch den Button
            item->setTreeButton(button);
            // Item anzeigen
            m_ui->trvConnections->addTopLevelItem(item);
            // Objekt ist nicht verbunden
            button->setReadyToConnect();
            // Alles erzeugt, Click auslösen            
            QTimer::singleShot(250, button, SLOT(click()));
        }        

        if(Settings::getInstance()->popUpDialog()) {
            showMe = true;
        }

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

    if (showMe) {
        this->show();
    }

    this->setIcon();
}

void Preferences::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible()) {
        hide();
        event->ignore();
    }
}

void Preferences::openInfo()
{
    AppInfo::getInstance()->show();
}

void Preferences::on_cmdClose_clicked()
{
    this->close();
}

void Preferences::manageConnections ()
{
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
        foreach (ListObject i, Configs::getInstance()->getConfigsObjects()) {
            OpenVpn *configObj = i.second;
            // Make new Menu from Config
            QMenu *trayTest = new QMenu(configObj->getConfigName(), this);
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
            mySubAction->setObjectName(configObj->getConfigName());
            connect(mySubAction, SIGNAL(triggered()), configObj, SLOT(openVpnLog()));
            trayTest->addAction (mySubAction);

            mySubAction = new QAction(QIcon(":/images/edit.png"), tr ("Quick edit"), this);
            mySubAction->setObjectName(configObj->getConfigName());
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
    QObject::connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayActivated(QSystemTrayIcon::ActivationReason)));
    this->setIcon();
}

void Preferences::createActions()
{
    // Normale Actions
    preferencesAction = new QAction(QIcon(":/images/status.png"), QObject::tr("&Show profiles"), this);
    preferencesAction->setEnabled(!this->isVisible());
    QObject::connect(preferencesAction, SIGNAL(triggered()), this, SLOT(manageConnections()));

    if (Settings::getInstance()->getIsPortableClient() || Settings::getInstance()->getIsManageClient()) {
        proxyAction = new QAction (QIcon(":/images/proxy.png"), QObject::tr("&Proxy Settings"), this);
        QObject::connect(proxyAction, SIGNAL(triggered()), this, SLOT(openProxySettings()));

        appInfoAction = new QAction (QIcon(":/images/info.png"),tr("&Information"), this);
        QObject::connect (appInfoAction, SIGNAL(triggered()), this, SLOT(openAppInfo()));
    }

    quitAction = new QAction(QIcon(":/images/system-log-out.png"),tr("&Quit"), this);
    QObject::connect(quitAction, SIGNAL(triggered()), this, SLOT(closeApp()));
}

void Preferences::closeApp()
{
    // Prüfen, ob noch eine Verbindung aktiv ist
    if (Settings::getInstance()->getIsRunAsSevice()) {
        for (int x = 0; x < m_ui->trvConnections->invisibleRootItem()->childCount(); x++) {
            // Item und OpenVpn Objekt holen
            TreeConItem *item = dynamic_cast<TreeConItem*>(m_ui->trvConnections->invisibleRootItem()->child(x));
            OpenVpn *vpn = item->getOpenVPN();
            if (vpn->isConnectionStable() || vpn->isConnecting()) {
                Message::warning(QObject::tr("You are still connected. Please disconnect first."), QObject::tr("A connection is still connected!"));
                return;
            }
        }
    } else {
        foreach (ListObject i, Configs::getInstance()->getConfigsObjects()) {
            OpenVpn *configObj = i.second;
            if (configObj->isConnectionStable() || configObj->isOnConnect()) {
                if (Message::confirm(QObject::tr("Do you want to disconnect the connection?"), QObject::tr("A connection is still connected!"))) {
                    configObj->disconnectVpn();
                }
            }
        }
    }

    if (Settings::getInstance()->getIsPortableClient()) {
#ifdef Q_OS_WIN32
        if (TapDriver::instance()->isTapDriverInstalled()) {
            if (Message::confirm(QObject::tr("Uninstall the driver?\nFor this action you need administrator permissions!"), QObject::tr("Remove Tap-Win32 driver"))) {
                if (!TapDriver::instance()->removeTapDriver()) {
                    Message::error(QObject::tr("Unable to uninstall Tap-Win32 driver!\nMaybe you have no permissions.\nPlease contact your system administrator."), QObject::tr("Securepoint SSL VPN"));
                    QCoreApplication::exit(1);
                }
            } else {
                 QCoreApplication::exit(1);
            }
        }
#endif
    }
    QCoreApplication::exit(0);
}

void Preferences::setIcon()
{
    //
    // Die Connection liste durchlaufen und nach connectionstate das icon setzen
    //

    bool configError (false);
    bool configConnecting (false);
    bool configConnected (false);

    // Alle TreeItem durchsuchen bis die ID gefunden wurde
    for (int x = 0; x < m_ui->trvConnections->invisibleRootItem()->childCount(); x++) {
        // Item und OpenVpn Objekt holen
        TreeConItem *item = dynamic_cast<TreeConItem*>(m_ui->trvConnections->invisibleRootItem()->child(x));
        TreeButton *but = item->getTreeButton();
        switch (but->getState()) {
            case 1:
                configConnecting = true;
                break;
            case 2:
                configConnected = true;
                break;
            case 4:
                configError = true;
                break;
            default:
                break;
                // Nix
        }
    }

    // Nun die Kobinationen auswerten
    QIcon icon (QIcon(":/images/state/idle.png"));

    if (configConnected) {
        icon = QIcon(":/images/state/connected.png");
    }

    if (configConnecting) {
        icon = QIcon(":/images/state/connecting.png");
    }

    if (configError) {
        icon = QIcon(":/images/state/error.png");
    }

    if (configConnecting && configError) {
        icon = QIcon(":/images/state/error-connecting.png");
    }

    if (configConnected && configError) {
        icon = QIcon(":/images/state/connected-error.png");
    }

    if (configConnected && configConnecting) {
        icon = QIcon(":/images/state/connect-connecting.png");
    }

    if (configConnected && configConnecting && configError) {
        icon = QIcon(":/images/state/connect-connecting-error.png");
    }

    this->trayIcon->setIcon(icon);
}

void Preferences::showTrayMessage(const QString &message, QSystemTrayIcon::MessageIcon MessageType, int duration)
{
    this->trayIcon->showMessage(QObject::tr("Securepoint SSL VPN"), message, MessageType, duration);
}

void Preferences::showTrayMessageChecked(const QString &message, QSystemTrayIcon::MessageIcon messageType, int duration)
{    
    if (!this->isVisible()) {        
        if (!Settings::getInstance()->getIsShowNoBallonMessage()) {            
            this->trayIcon->showMessage(QObject::tr("Securepoint SSL VPN"), message, messageType, duration);
        }
    }
}

void Preferences::on_cmdOpenInfo_clicked()
{
    AppInfo::getInstance()->show();
}

void Preferences::on_trvConnections_itemDoubleClicked(QTreeWidgetItem* item, int column)
{
    if (column == 1) {
        if (Settings::getInstance()->getIsManageClient()) {
            TreeConItem* _item(dynamic_cast<TreeConItem*>(item));
            if (_item) {
                _item->getOpenVPN()->openManageConnection();
            }
        }
    } else if (column == 2) {
        if (Settings::getInstance()->getIsRunAsSevice()) {
            TreeConItem *_item = dynamic_cast<TreeConItem*>(item);
            if (_item) {                
                ServiceLog logDialog (_item->getOpenVPN()->id());
                logDialog.exec();
            }
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
    if (!Settings::getInstance()->getIsManageClient()) {
        m_ui->tab_4->setEnabled(false);
    }

    if (!m_ui->extensionWidget->isVisible()) {
        m_ui->extensionWidget->setVisible(true);
        m_ui->tabWidget->setCurrentIndex(0);
        this->setMaximumHeight(605);
        this->setMinimumHeight(605);
    } else {
        m_ui->extensionWidget->setVisible(false);
        this->setMinimumHeight(390);
        this->setMaximumHeight(390);
    }
}

void Preferences::openProxySettings()
{
    ProxySettings::getInstance()->show();
}

void Preferences::openAppInfo() {
    AppInfo::getInstance()->show();
}

void Preferences::on_cmdNewConfig_clicked()
{
    // Nun den Wizard aufrufen
    VpnWizard wiz;
    wiz.exec();
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
            RenameConfig::getInstance()->setOldName(item->getOpenVPN()->getConfigName());
            RenameConfig::getInstance()->setFullPath(item->getOpenVPN()->getConfigPath() + QLatin1String("/") + item->getOpenVPN()->getConfigName() + QLatin1String(".ovpn"));

            QMenu configPopUp (this);            
            bool toggleMenues (true);
            // Connect
            if (item->getOpenVPN()->isConnecting() || item->getOpenVPN()->isConnectionStable()) {
                configPopUp.addAction(QPixmap(":/images/disconnetdmen.png"), QObject::tr("&Disconnect"), item->getTreeButton(), SLOT(click()));
            } else {
                configPopUp.addAction(QPixmap(":/images/connectmen.png"), QObject::tr("Con&nect"), item->getTreeButton(), SLOT(click()));
            }            
            configPopUp.addSeparator();
            // Delete
            // Wenn die Verbindung hergestellt ist oder gerade beim connecten
            // den Menupunkt deaktivieren
            if (item->getOpenVPN()->isConnecting() || item->getOpenVPN()->isConnectionStable()) {
                toggleMenues = false;
            }            

            if (item->getOpenVPN()->isConfigLinked()) {
                configPopUp.addAction(QPixmap(":/images/delete.png"), QObject::tr("&Delete"), this, SLOT(deleteLinkedConfig()))->setEnabled(toggleMenues);
            } else {
                configPopUp.addAction(QPixmap(":/images/delete.png"), QObject::tr("&Delete"), DeleteConfig::getInstance(), SLOT(show()))->setEnabled(toggleMenues);
            }

            // Startconfig
            configPopUp.addSeparator();
            if (item->getOpenVPN()->getConfigFullPath() == Settings::getInstance()->getCommandConfigPath()) {
                configPopUp.addAction(QPixmap(":/images/releasestart.png"), QObject::tr("&Release"), this, SLOT(removeStartConfig()))->setEnabled((item->getTreeButton()->getState() != 1));
            } else {
                configPopUp.addAction(QPixmap(":/images/setstart.png"), QObject::tr("&Start configuration"), this, SLOT(setStartConfig()));
            }

            if (item->getOpenVPN()->isError() && item->getTreeButton()->getState() == 4) {
                configPopUp.addSeparator();
                configPopUp.addAction(QPixmap(":/images/list-remove.png"), QObject::tr("&Clear error"), this, SLOT(removeErrorFromConfig()));
            }

            if (item->getOpenVPN()->hasCrediantials()) {
                configPopUp.addSeparator();
                configPopUp.addAction(QPixmap(":/images/list-remove.png"), QObject::tr("&Clear saved data"), item->getOpenVPN(), SLOT(removeCredentials()));
            }

            configPopUp.addSeparator();
            configPopUp.addAction(QPixmap(":/images/rename.png"), QObject::tr("&Rename"), RenameConfig::getInstance(), SLOT(show()))->setEnabled(toggleMenues);
            configPopUp.addSeparator();
            configPopUp.addAction(QPixmap(":/images/manage.png"), QObject::tr("&Edit"), item->getOpenVPN(), SLOT(openManageConnection()))->setEnabled(toggleMenues);
            configPopUp.addAction(QPixmap(":/images/edit.png"), QObject::tr("&Quick edit"), item->getOpenVPN(), SLOT(openEditConfig()))->setEnabled(toggleMenues);
            configPopUp.addSeparator();
            configPopUp.addAction(QPixmap(":/images/export.png"), QObject::tr("&Export"), item->getOpenVPN(), SLOT(openExport()));

            configPopUp.addAction(QPixmap(":/images/import.png"), QObject::tr("&Import"), this, SLOT(on_cmdImportConfig_clicked()));
            configPopUp.addSeparator();
            configPopUp.exec(m_ui->trvConnections->mapToGlobal(pos));
        }
    }
}

void Preferences::removeStartConfig()
{
    for (int x = 0; x < m_ui->trvConnections->invisibleRootItem()->childCount(); x++) {
        // Item und OpenVpn Objekt holen
        TreeConItem *item = dynamic_cast<TreeConItem*>(m_ui->trvConnections->invisibleRootItem()->child(x));
        OpenVpn *vpn = item->getOpenVPN();
        vpn->setAdvName(QLatin1String(""));
    }
    Settings::getInstance()->setIsStartCommandConfig (false, true);
    Settings::getInstance()->setCommandConfigPath(QLatin1String(""), true);
    this->refreshDialog();
}

void Preferences::setStartConfig()
{
    // Alle Startconfigs entfernen
    for (int x = 0; x < m_ui->trvConnections->invisibleRootItem()->childCount(); x++) {
        // Item und OpenVpn Objekt holen
        TreeConItem *item = dynamic_cast<TreeConItem*>(m_ui->trvConnections->invisibleRootItem()->child(x));
        OpenVpn *vpn = item->getOpenVPN();
        vpn->setAdvName(QLatin1String(""));
    }

    TreeConItem *item = NULL;
    item = dynamic_cast<TreeConItem*>(m_ui->trvConnections->currentItem());
    if (item) {
        Settings::getInstance()->setIsStartCommandConfig (true, true);
        Settings::getInstance()->setCommandConfigPath(item->getOpenVPN()->getConfigFullPath(), true);
        item->getOpenVPN()->setAdvName(QLatin1String ("\n") + QObject::tr("- Start config"));
        this->refreshDialog();
    }
}

void Preferences::removeErrorFromConfig()
{
    TreeConItem *item = NULL;
    item = dynamic_cast<TreeConItem*>(m_ui->trvConnections->currentItem());
    if (item) {
        item->getOpenVPN()->setIsError(false);
        item->getOpenVPN()->setErrorString(QLatin1String(""));
        this->refreshDialog();
        this->setIcon();
    }
}

void Preferences::deleteLinkedConfig()
{
    TreeConItem *item = NULL;
    item = dynamic_cast<TreeConItem*>(m_ui->trvConnections->currentItem());
    if (item) {
        if (item->getOpenVPN()->getConfigFullPath() == Settings::getInstance()->getCommandConfigPath()) {
            Message::error(QObject::tr("Can't delete a start config. Please release it first."));
            return;
        }

        if (Message::confirm(QObject::tr("Remove the linked configuration from list?"), QObject::tr("Delete linked configuration"))) {
            Configs::getInstance()->removeConfigFromList(item->getOpenVPN()->getConfigPath() + QLatin1String("/") + item->getOpenVPN()->getConfigName() + QLatin1String(".ovpn"));
            this->refreshConfigList();
            this->setConnectionStatus();
            this->refreshDialog();
        }
    }
}

void Preferences::on_cmdRefreshData_clicked()
{
    this->refreshConfigList();
    this->setConnectionStatus();
    this->refreshDialog();
    this->setIcon();
}

void Preferences::on_cmdImportConfig_clicked()
{
    ImportConfig importDialog;
    importDialog.exec();
}

void Preferences::on_cmdToogleBallon_clicked()
{
    // Toggle Setting
    Settings::getInstance()->setIsShowNoBallonMessage(!Settings::getInstance()->getIsShowNoBallonMessage());
    m_ui->lblShowBallon->setText((Settings::getInstance()->getIsShowNoBallonMessage() ? QObject::tr("no") : QObject::tr("yes") ));
}

void Preferences::on_cmdTooglePopup_clicked()
{
    Settings::getInstance()->setPopUpDialog(!Settings::getInstance()->popUpDialog());
    m_ui->lblNoPopUp->setText((Settings::getInstance()->popUpDialog() ? QObject::tr("yes") : QObject::tr("no") ));
}

void Preferences::on_cmdToogleReconnect_clicked()
{
    Settings::getInstance()->setIsAutoReconnect(!Settings::getInstance()->getIsAutoReconnect());
    m_ui->lblReconnect->setText((Settings::getInstance()->getIsAutoReconnect() ? QObject::tr("yes") : QObject::tr("no") ));
}

void Preferences::on_cmdAddNewTap_clicked()
{

    if (this->isConnectionActive()) {
        if (!Message::confirm(QObject::tr("A connection is online. While installing all connection will be disconnected! Do you wish to continue?"))) {
            return;
        }
    }

    // Neues Tap installieren
    if(Settings::getInstance()->getIsRunAsSevice()) {
        // Befehl an den Dienst senden
        QApplication::setOverrideCursor(Qt::WaitCursor);
        SrvCLI::instance()->send(QLatin1String("ADDTAP"), QLatin1String(""));
    } else {
        // Ohne Dienst
        if (Message::confirm(QObject::tr("Install the driver?\nFor this action you need administrator permissions!"), QObject::tr("Securepoint SSL VPN"))) {
            QApplication::setOverrideCursor(Qt::WaitCursor);
            if (!TapDriver::instance()->installTapDriver()) {
                QApplication::setOverrideCursor(Qt::ArrowCursor);
                Message::error(QObject::tr("Unable to install Tap-Win32 driver!\nMaybe you have no permissions.\nPlease contact your system administrator."), QObject::tr("Install Tap Device"));
            }
            QApplication::setOverrideCursor(Qt::ArrowCursor);
        }
    }
}

void Preferences::receivedRemoveTap(QString state)
{
    if (state == QLatin1String("OK")) {
        QApplication::setOverrideCursor(Qt::ArrowCursor);
        Message::information(QObject::tr(("Tap devices remove successfully.")));
    } else if (state == QLatin1String("ERROR")) {
        QApplication::setOverrideCursor(Qt::ArrowCursor);
        Message::error(QObject::tr("Unable to uninstall Tap-Win32 driver!\nMaybe you have no permissions.\nPlease contact your system administrator."), QObject::tr("Securepoint SSL VPN"));
    }
}


void Preferences::on_cmdRemoveAllTap_clicked()
{
    if (this->isConnectionActive()) {
        if (!Message::confirm(QObject::tr("A connection is online. While removing tap devices all connection will be disconnected! Do you wish to continue?"))) {
            return;
        }
    }

    // Alle Tap devices löschen
    if(Settings::getInstance()->getIsRunAsSevice()) {
        // Befehl an den Dienst senden
        QApplication::setOverrideCursor(Qt::WaitCursor);
        SrvCLI::instance()->send(QLatin1String("REMOVETAP"), QLatin1String(""));
    } else {
        // Ohne Dienst
        if (Message::confirm(QObject::tr("Uninstall the driver?\nFor this action you need administrator permissions!"), QObject::tr("Remove Tap-Win32 driver"))) {
            QApplication::setOverrideCursor(Qt::WaitCursor);
            if (!TapDriver::instance()->removeTapDriver()) {
                QApplication::setOverrideCursor(Qt::ArrowCursor);
                Message::error(QObject::tr("Unable to uninstall Tap-Win32 driver!\nMaybe you have no permissions.\nPlease contact your system administrator."), QObject::tr("Securepoint SSL VPN"));
            }
            QApplication::setOverrideCursor(Qt::ArrowCursor);
        }
    }
}

void Preferences::on_cmdToogleUseInteract_clicked()
{
    Settings::getInstance()->setUseNoInteract(!Settings::getInstance()->getUseNoInteract());
    m_ui->lblUseInteract->setText((Settings::getInstance()->getUseNoInteract() ? QObject::tr("yes") : QObject::tr("no") ));
}

void Preferences::on_cmdRemoveCredentials_clicked()
{
    if (Message::confirm(QObject::tr("Do you really want to remove all saved credentials?"))) {
        // Alle Startconfigs entfernen
        for (int x = 0; x < m_ui->trvConnections->invisibleRootItem()->childCount(); x++) {
            // Item und OpenVpn Objekt holen
            TreeConItem *item = dynamic_cast<TreeConItem*>(m_ui->trvConnections->invisibleRootItem()->child(x));
            OpenVpn *vpn = item->getOpenVPN();
            vpn->removeCredentials(false);
        }
        this->refreshDialog();
    }
}

void Preferences::on_cmdShowCredentials_clicked()
{
    // Die Liste zusammensetzen
    // type 0=user;1=pass;3=pkcs12 wird benötigt
    QStringList data;
    for (int x = 0; x < m_ui->trvConnections->invisibleRootItem()->childCount(); x++) {
        // Item und OpenVpn Objekt holen
        TreeConItem *item = dynamic_cast<TreeConItem*>(m_ui->trvConnections->invisibleRootItem()->child(x));
        OpenVpn *vpn = item->getOpenVPN();
        QString line (QString("%1;%2;%3;%4;%5;%6;%7")
                      .arg(vpn->id())
                      .arg(vpn->getConfigName())
                      .arg((vpn->hasCrediantials(0) ? QLatin1String("1") : QLatin1String("0")))
                      .arg((vpn->hasCrediantials(1) ? QLatin1String("1") : QLatin1String("0")))
                      .arg((vpn->hasCrediantials(3) ? QLatin1String("1") : QLatin1String("0")))
                      .arg((vpn->hasCrediantials(5) ? QLatin1String("1") : QLatin1String("0")))
                      .arg((vpn->hasCrediantials(6) ? QLatin1String("1") : QLatin1String("0")))
                      );
        data.append(line);
    }
    FrmSavedData dialog (data);
    dialog.exec();
}

void Preferences::on_cmdToogleSpalshScreen_clicked()
{
    Settings::getInstance()->setShowSplashScreen(!Settings::getInstance()->showSplashScreen());
    m_ui->lblShowSplash->setText((Settings::getInstance()->showSplashScreen() ? QObject::tr("yes") : QObject::tr("no")));
}

void Preferences::on_cmdToogleStartup_clicked()
{
    Settings::getInstance()->setAutoStartOnWindowsStartup(!Settings::getInstance()->autoStartOnWindowsStartup());
    m_ui->lblStartupWindows->setText((Settings::getInstance()->autoStartOnWindowsStartup() ? QObject::tr("yes") : QObject::tr("no")));
}

void Preferences::on_cmbDelay_currentIndexChanged(int index)
{    
    if (index == 0) {
        Settings::getInstance()->setDelayStartConfig(QLatin1String("0"));
    } else if (index == 1) {
        Settings::getInstance()->setDelayStartConfig(QLatin1String("30"));
    } else if (index == 2) {
        Settings::getInstance()->setDelayStartConfig(QLatin1String("60"));
    } else if (index == 3) {
        Settings::getInstance()->setDelayStartConfig(QLatin1String("90"));
    } else if (index == 4) {
        Settings::getInstance()->setDelayStartConfig(QLatin1String("120"));
    }
}
