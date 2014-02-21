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
#include "frmupdatesettings.h"

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
    this->setMinimumHeight(380);
    this->setMaximumHeight(380);

    this->setWindowFlags(Qt::WindowCloseButtonHint);

    // Banner setzen
    // Wenn eine Banner png da ist das Banner neu setzen
    QString bannerPath(QCoreApplication::applicationDirPath() + QLatin1String("/banner.png"));
    //
    if (QFile::exists(bannerPath)) {
        // Datei laden
        QPixmap tempBanner (bannerPath);
        // Nun die Bild größe überprüfen es muss 480x320 sein
        if (tempBanner.height() == 63 && tempBanner.width() == 445) {
            // Alles Ok, das Bild setzen
            m_ui->cmdOpenInfo->setIcon(QIcon(tempBanner));
        }
    }

    // Set window title
    QString titleAdd;
    // Ist der Client mit manage aufgerufen?
    if (Settings::getInstance()->getIsManageClient()) {
        titleAdd = QObject::tr(" - managed");
    } else if (Settings::getInstance()->getIsPortableClient()) {
        titleAdd = QObject::tr(" - portable");
    }

    this->setWindowTitle(QObject::tr("Securepoint OpenVPN v1.0.1") + titleAdd);

    // Set DB
    Configs::getInstance()->setDatabase(&this->db);

    // Set Build
    this->internalBuildValue = QLatin1String("1.0.1");

    // Update
    m_ui->cmdOpenUpdate->setEnabled(false);
    if (Settings::getInstance()->isAutoUpdate()) {
        QTimer::singleShot(3000, this, SLOT(on_cmdCheckUpdate_clicked()));
    }

    //
    this->refreshTapDeviceCount();
}

void Preferences::searchStartConfigDir()
{
    this->refreshConfigList();
    createActions();
    createTrayIcon();
    trayIcon->show();

    this->refreshDialog();

    // Auto connect?
    for (int x = 0; x < m_ui->trvConnections->invisibleRootItem()->childCount(); x++) {
        TreeConItem *item = dynamic_cast<TreeConItem*>(m_ui->trvConnections->invisibleRootItem()->child(x));
        if (item->getOpenVPN()->isStartConfig()) {
            QTimer::singleShot(250, item->getTreeButton(), SLOT(click()));
        }
    }
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
        this->refreshTapDeviceCount();
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

        if (obj->isConnectionStable() || obj->isConnecting()) {
            return true;
        }
    }

    return false;
}

void Preferences::closeAllOpenConnections()
{
    // Ist eine Verbindung gerade beim verbinden oder schon verbunden?
    // Dann die Verbindung trennen, da der Benutzer sich abmeldet bzw. neustartet/ herunterfährt
    // Wird in der single_application im Event-Filter für die WM_QUERYENDSESSION
    // gebraucht.

    foreach (ListObject i, Configs::getInstance()->getConfigsObjects()) {
        OpenVpn* obj = i.second;

        if (obj->isConnectionStable() || obj->isConnecting()) {
            //
            // Script vor dem Disconnect ausführen
            obj->runScript(QLatin1String("BD"));
            //
            // Nun den Disconnect senden
            if (Settings::getInstance()->getIsRunAsSevice()) {
                SrvCLI::instance()->send(QLatin1String("Close"), QString::number(obj->id()));
            } else {
                obj->disconnectVpn();
            }
        }
    }
}

void Preferences::trayActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::DoubleClick) {
        if (!this->isMinimized()) {
            if (!this->isVisible()) {
                this->refreshDialog();
                this->setIcon();
            }
            //
            if (Settings::getInstance()->getIsPortableClient() || Settings::getInstance()->getIsManageClient()) {
                m_ui->cmdNewConfig->show();
                m_ui->cmdImportConfig->show();
                m_ui->cmdRefreshData->show();
            } else {
                m_ui->cmdNewConfig->hide();
            }
            this->showNormal();
            this->setFocus();
            this->activateWindow();
        }
    }
}

void Preferences::refreshConfigList()
{
    // Liste neu einlesen
    Configs::getInstance()->refreshConfigs();
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
        if (obj->isConnectionStable() || obj->isConnecting()) {
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
        item->setText(1, configObj->getConfigName() + configObj->getAdvName());
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
                    SrvCLI::instance()->send(QLatin1String("STATUS"), QString("%1").arg(configObj->id()));
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


    if (Configs::getInstance()->configCount() > 1) {
        m_ui->trvConnections->setSelectionMode(QAbstractItemView::SingleSelection);
    } else {
        m_ui->trvConnections->setSelectionMode(QAbstractItemView::NoSelection);
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
    m_ui->lblWinEvent->setText((Settings::getInstance()->checkWindowsShutdown() ? QObject::tr("yes") : QObject::tr("no")));
    m_ui->lblShowSplash->setText((Settings::getInstance()->showSplashScreen() ? QObject::tr("yes") : QObject::tr("no")));
    m_ui->lblUseInteract->setText((Settings::getInstance()->getUseNoInteract() ? QObject::tr("yes") : QObject::tr("no") ));

    m_ui->lblLanguage->setText((Settings::getInstance()->getIsLanguageGerman() ? QObject::tr ("German") : QObject::tr ("English")));

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
    // Status
    QObject::connect(this->server, SIGNAL(receivedStatus(int,bool,bool,int,QString)), this, SLOT(receivedStatus(int,bool,bool,int,QString)));

    // Nun die Verbindung testen
    int tries (0);
    bool success (false);

    do {
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
            success = true;
        } else {
            // Timeout
            success = false;
            ++tries;
        }
        // Disconnect dummy signal
        QObject::disconnect(this->server, SIGNAL(receivedDummy()), 0, 0);
        QObject::disconnect(&timeoutTimer, 0, 0, 0);
        // Sleep 1000ms
        if (qApp->hasPendingEvents()) {
            qApp->processEvents();
        }
        QThreadExec::Sleep(1000);
        //
        Debug::log(QString(QString("Pref: Connect try: No.: %1 Success: %2")
                           .arg(tries)
                           .arg(success)));

    } while ((!success) && (tries < 5));


    return success;
}

void Preferences::receivedStatus(int id, bool isConnected, bool isConnecting, int lastAction, QString ip)
{
    for (int x = 0; x < m_ui->trvConnections->invisibleRootItem()->childCount(); x++) {
        // Item und OpenVpn Objekt holen
        TreeConItem *item = dynamic_cast<TreeConItem*>(m_ui->trvConnections->invisibleRootItem()->child(x));
        OpenVpn *vpn = item->getOpenVPN();
        // Nun die Daten speichern
        if (vpn->id() == id) {
            if (isConnecting) {
                vpn->setIsConnected(false);
                vpn->setIsConnecting(true);
                item->getTreeButton()->setConnecting();
            } else if (isConnected) {
                vpn->setIsConnected(true);
                vpn->setIsConnecting(false);
                vpn->setConnectedIP(ip);
                item->getTreeButton()->setConnected(ip);
            }
            if (lastAction > -1) {
                // Eine benutzeraktion wird erwartet
                this->userInputIsNeeded(vpn->id(), lastAction);
            }
            this->setIcon();
            break;
        }
    }
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

    QString cName("n/a");
    for (int x = 0; x < m_ui->trvConnections->invisibleRootItem()->childCount(); x++) {
        // Item und OpenVpn Objekt holen
        TreeConItem *item = dynamic_cast<TreeConItem*>(m_ui->trvConnections->invisibleRootItem()->child(x));
        OpenVpn *vpn = item->getOpenVPN();

        // Nun die Daten speichern
        if (vpn->id() == id) {

            cName = vpn->getConfigName();
            if (vpn->hasCrediantials(type)) {

                // Das ist ein bischen doof hier, geht aber nicht anderes ohne den client neu zu designen
                QString value (vpn->getSavedUserData(type));

                SrvCLI::instance()->send(command, QString("%1;%2").arg(id).arg(value));
                return;
            }
        }
    }

    // Es sind keine Daten gespeichert, neue abfragen
    FrmGetUserData dialog (ntype, cName, id);
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

void Preferences::openDialog()
{
    bool visibleState (Settings::getInstance()->getIsPortableClient() || Settings::getInstance()->getIsManageClient());
    m_ui->cmdNewConfig->setVisible(visibleState);
    m_ui->cmdImportConfig->setVisible(true);
    m_ui->cmdRefreshData->setVisible(true);

    if (Settings::getInstance()->popUpDialog()) {
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

    // Save window position
    Settings::getInstance()->setWindowTop(this->geometry().topLeft());
}

void Preferences::showEvent(QShowEvent *event)
{
    QPoint topLeft = Settings::getInstance()->windowTop();
    if (!topLeft.isNull()) {
        // Setzte Fenster Position
        if (topLeft.x() <= 0 || topLeft.x() + 100 > qApp->desktop()->width()) {
            this->centerMidle();
            event->accept();
            return;
        }

        if (topLeft.y() <= 0 || topLeft.y() + 100 > qApp->desktop()->height()) {
            this->centerMidle();
            event->accept();
            return;
        }

        this->setGeometry(topLeft.x(), topLeft.y(), this->width(), this->height());
    }

    event->accept();
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
    this->openDialog();
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
                    this->trayIcon->hide();
                    QCoreApplication::processEvents();
                    QCoreApplication::exit(1);
                }
            } else {
                this->trayIcon->hide();
                QCoreApplication::processEvents();
                 QCoreApplication::exit(1);
            }
        }
#endif
    }

    this->trayIcon->hide();
    QCoreApplication::processEvents();
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

        if (Settings::getInstance()->getIsPortableClient()) {
            m_ui->tab_4->setEnabled(true);
        }
    }

    if (!m_ui->extensionWidget->isVisible()) {
        m_ui->extensionWidget->setVisible(true);
        m_ui->tabWidget->setCurrentIndex(0);
        this->setMaximumHeight(590);
        this->setMinimumHeight(590);
        m_ui->cmdToggleExtensions->setIcon(QIcon(":/images/tree_closed.png"));
        m_ui->cmdToggleExtensions->setToolTip(QObject::tr("Hide advanced settings"));
    } else {
        m_ui->extensionWidget->setVisible(false);
        m_ui->cmdToggleExtensions->setIcon(QIcon(":/images/tree_expand.png"));
        m_ui->cmdToggleExtensions->setToolTip(QObject::tr("Show advanced settings"));
        this->setMinimumHeight(380);
        this->setMaximumHeight(380);
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
    TreeConItem *item = NULL;
    item = dynamic_cast<TreeConItem*>(m_ui->trvConnections->itemAt(pos));
    if (item) {
        // Dialoge inititiieren
        DeleteConfig::getInstance()->setOpenVpnObject(item->getOpenVPN());
        RenameConfig::getInstance()->setOldName(item->getOpenVPN()->getConfigName());
        RenameConfig::getInstance()->setId(item->getOpenVPN()->id());

        QMenu configPopUp (this);
        bool toggleMenues (true);
        bool enableMenues (true);
        bool enableDelete (true);
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
            enableDelete = false;
        }

        if (!Settings::getInstance()->getIsManageClient()) {
            toggleMenues = false;
            enableMenues = false;
        }


        //
        configPopUp.addAction(QPixmap(":/images/delete.png"), QObject::tr("&Delete"), DeleteConfig::getInstance(), SLOT(show()))->setEnabled(enableDelete);


        // Startconfig
        configPopUp.addSeparator();

        if (item->getOpenVPN()->isStartConfig()) {
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
        configPopUp.addAction(QPixmap(":/images/export.png"), QObject::tr("&Export"), item->getOpenVPN(), SLOT(openExport()))->setEnabled(enableMenues);

        configPopUp.addAction(QPixmap(":/images/import.png"), QObject::tr("&Import"), this, SLOT(on_cmdImportConfig_clicked()))->setEnabled(enableMenues);
        configPopUp.addSeparator();
        configPopUp.exec(m_ui->trvConnections->mapToGlobal(pos));
    }
}

void Preferences::removeStartConfig()
{
    TreeConItem *item = NULL;
    item = dynamic_cast<TreeConItem*>(m_ui->trvConnections->currentItem());
    if (item) {
        item->getOpenVPN()->setStartConfig(false);
        QString sql (QString("UPDATE vpn SET [vpn-autostart] = 0 WHERE [vpn-id] = %1")
                     .arg(item->getOpenVPN()->id()));
        this->db.execute(sql);
        this->refreshDialog();
    }
}

void Preferences::setStartConfig()
{
    TreeConItem *item = NULL;
    item = dynamic_cast<TreeConItem*>(m_ui->trvConnections->currentItem());
    if (item) {
        item->getOpenVPN()->setStartConfig(true);
        QString sql (QString("UPDATE vpn SET [vpn-autostart] = 1 WHERE [vpn-id] = %1")
                     .arg(item->getOpenVPN()->id()));
        this->db.execute(sql);
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
            this->refreshTapDeviceCount();
            QApplication::setOverrideCursor(Qt::ArrowCursor);
        }
    }
}

void Preferences::receivedRemoveTap(QString state)
{
    if (state == QLatin1String("OK")) {
        QApplication::setOverrideCursor(Qt::ArrowCursor);
        Message::information(QObject::tr(("Tap devices remove successfully.")));
        this->refreshTapDeviceCount();
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
        //
        if(Message::confirm(QObject::tr("Do you realy want to remove all TAP device?"))) {
            // Befehl an den Dienst senden
            QApplication::setOverrideCursor(Qt::WaitCursor);
            SrvCLI::instance()->send(QLatin1String("REMOVETAP"), QLatin1String(""));
        }
    } else {
        // Ohne Dienst
        if (Message::confirm(QObject::tr("Uninstall the driver?\nFor this action you need administrator permissions!"), QObject::tr("Remove Tap-Win32 driver"))) {
            QApplication::setOverrideCursor(Qt::WaitCursor);
            if (!TapDriver::instance()->removeTapDriver()) {
                QApplication::setOverrideCursor(Qt::ArrowCursor);
                Message::error(QObject::tr("Unable to uninstall Tap-Win32 driver!\nMaybe you have no permissions.\nPlease contact your system administrator."), QObject::tr("Securepoint SSL VPN"));
            }
            this->refreshTapDeviceCount();
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

void Preferences::on_cmdToogleWinEvent_clicked()
{
    Settings::getInstance()->setCheckWindowsShutdown(!Settings::getInstance()->checkWindowsShutdown());
    m_ui->lblWinEvent->setText((Settings::getInstance()->checkWindowsShutdown() ? QObject::tr("yes") : QObject::tr("no")));
}

void Preferences::renameConfig(int id, const QString &newName)
{
    //
    // Rename config in db
    //

    QString sql (QString("UPDATE vpn SET [vpn-name] = '%1' WHERE [vpn-id] = %2")
                 .arg(this->db.makeCleanValue(newName))
                 .arg(id));
    this->db.execute(sql);
}

void Preferences::removeConfigInDatabase(int id)
{
    //
    // Remove config in db
    //

    QString sql (QString("DELETE FROM vpn WHERE [vpn-id] = %1")
                 .arg(id));
    this->db.execute(sql);
}

void Preferences::addNewConfigToDatabase(const QString &name, const QString &path)
{
    //
    // Add a new config into db
    //

    QString sql (QString("INSERT INTO vpn ([vpn-name], [vpn-config], [vpn-autostart]) VALUES ('%1', '%2', 0);")
             .arg(this->db.makeCleanValue(name))
             .arg(this->db.makeCleanValue(path).replace("\\", "/")));
    this->db.execute(sql);
}

void Preferences::refreshTapDeviceCount()
{
    qApp->setOverrideCursor(Qt::WaitCursor);
    m_ui->lblTapCount->setText(QString::number(TapDriver::instance()->deviceCount()));
    qApp->setOverrideCursor(Qt::ArrowCursor);
}

void Preferences::centerMidle()
{
    int winW = this->width();
    int winH = this->height();

    int left (0);
    int top (0);

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

    // Nun die neuen setzen
    this->setGeometry(left, top, winW, winH);
}

void Preferences::on_cmdCheckUpdate_clicked()
{
    //
    // Check for update
    //

    ///delete this->update;
    //this->update = 0;
    //
    this->update = new ParseXML;
    QObject::connect(this->update, SIGNAL(finished(bool, QString)), this, SLOT(updateCheckIsReady(bool, QString)));

    m_ui->cmdCheckUpdate->setEnabled(false);
    m_ui->cmdOpenUpdate->setEnabled(false);
    qApp->setOverrideCursor(Qt::WaitCursor);

    // Set movie
    QMovie *movie = new QMovie(":/images/loading.gif");
    m_ui->lblUpdateState->setMovie(movie);
    movie->start();

    this->update->runParser();
}

void Preferences::updateCheckIsReady(bool success, QString errMessage)
{
    //
    // Update is finished
    //

    if (!success) {
        m_ui->lblUpdateState->setText(QObject::tr("Error") + QLatin1String(": ") + errMessage);
        m_ui->lblUpdateState->setToolTip(errMessage);
    } else {
        QString uVersion (this->update->highestVersion());
        if (this->internalBuild() == uVersion) {
            m_ui->lblUpdateState->setText(QObject::tr("Up to date.") + QLatin1String(" [") + uVersion + QLatin1String("]"));
        } else {
            // Check if it is a newer version
            QString thisVersion (this->internalBuild());
            QString netVersion (uVersion);
            // Remove dots
            thisVersion = thisVersion.remove(".");
            netVersion = netVersion.remove(".");
            // To int
            int dVersion = thisVersion.toInt();
            int dNetVersion = netVersion.toInt();
            //
            if (dNetVersion > dVersion) {
                m_ui->lblUpdateState->setText(QObject::tr("It is a newer version available.") + QLatin1String(" [") + uVersion + QLatin1String("]"));
                m_ui->cmdOpenUpdate->setEnabled(true);
                if (!m_ui->tab_5->isVisible()) {
                    qApp->setOverrideCursor(Qt::ArrowCursor);
                    if (Message::confirm(QObject::tr("It is a newer version available.") + QLatin1String(" [") + uVersion + QLatin1String("]\n") + QObject::tr("Do you want to update the vpn client?"), QObject::tr("Please confirm"), true)) {
                        QTimer::singleShot(0, this, SLOT(on_cmdOpenUpdate_clicked()));
                    }
                }
            } else {
                m_ui->lblUpdateState->setText(QObject::tr("Up to date.") + QLatin1String(" [") + uVersion + QLatin1String("]"));
            }
        }
    }

    m_ui->cmdCheckUpdate->setEnabled(true);
    qApp->setOverrideCursor(Qt::ArrowCursor);
}

QString Preferences::internalBuild() const
{
    //
    // Return the version
    //

    return this->internalBuildValue;
}

void Preferences::on_cmdUpdateSetting_clicked()
{
    //
    // Open Update settings
    //

    FrmUpdateSettings dialog;
    dialog.exec();
}

void Preferences::on_cmdOpenUpdate_clicked()
{
    //
    // Start update process
    //

    if (this->isConnectionActive()) {
        Message::warning(QObject::tr("Can't start update process with an active connection!"));
        return;
    }


    if (Settings::getInstance()->useSourceForge()) {
        QDesktopServices::openUrl(QUrl("http://sourceforge.net/projects/securepoint/files"));
    } else {
        QDesktopServices::openUrl(QUrl("http://download.securepoint.de"));
    }


    /*
    QStringList args;
    args << "-target";
    args << QString("http://updatevpnc.securepoint.de/%1.zip").arg(this->update->highestVersion());;
    if (Settings::getInstance()->getIsLanguageGerman()) {
        args << QLatin1String("-lang");
        args << QLatin1String("de");
    }

    //
    QSettings osset ("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", QSettings::NativeFormat);
    QString winver = osset.value("ProductName").toString();
    if (winver.contains("Windows 7", Qt::CaseInsensitive) || winver.contains("Vista", Qt::CaseInsensitive) || winver.contains("2008", Qt::CaseInsensitive)) {
        QString arguments (args.join(QLatin1String(" ")));
        wchar_t *apiParams = (wchar_t*) arguments.utf16();
        wchar_t *filename = (wchar_t*) QString (qApp->applicationDirPath().replace("/", "\\") + QLatin1String("\\updater\\updater.exe")).utf16();

        SHELLEXECUTEINFO ShellInfo = {0};
        memset(&ShellInfo, 0, sizeof(ShellInfo));
        ShellInfo.cbSize = sizeof(ShellInfo);
        ShellInfo.hwnd = NULL;
        ShellInfo.lpVerb =  L"runas";
        ShellInfo.lpFile =  filename;
        ShellInfo.lpParameters = apiParams;
        ShellInfo.nShow = SW_SHOWNORMAL;
        ShellInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
        bool exec = ShellExecuteEx(&ShellInfo);
        Q_UNUSED(exec)
    } else {
        // Klassischer Aufruf
        QProcess *updateProcess = new QProcess (this);
        updateProcess->setWorkingDirectory(QCoreApplication::applicationDirPath());
        updateProcess->start(QCoreApplication::applicationDirPath() + QLatin1String("/updater/updater.exe"), args);
    }
    */
}
