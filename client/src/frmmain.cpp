#include "frmmain.h"
#include "ui_frmmain.h"
#include <debug/debug.h>
#include <QGuiApplication>

#define _UNICODE
// Fix error in QDateTime
#ifndef NOMINMAX
#define NOMINMAX
#endif


// Include before Windows.h to avoid compile errors
//#include <WinSock2.h>
#include <Windows.h>

float windowsDpiScale();

// Definded in main.cpp
extern bool g_bSilent;
//
extern QString g_strClientName;

#include <tchar.h>
#include <QtCore/QDebug>
#include <QtWidgets/QMenu>
#include <QtGui/QPainter>
#include <utils.h>
#include <future>
#include <mutex>
#include <condition_variable>
#include <thread>

// Custom includes
#include "rightalignpushbuttonmenu.h"
#include "widgetfactory.h"
#include <widgets/settings/client/settings.h>
#include "dialogs\frmgetuserdata.h"
#include <service\srvcli.h>
#include <config/Configs.h>
#include <message.h>
#include <wizard\wiz_vpnwizard.h>
#include <database\crypt.h>
#include <database\database.h>
#include <proxysettings.h>
#include <updatecheck.h>

//
// Message IDs
#define ON_TB_MOVE 11052
#define ON_SEND_DUMMY 11053
#define FAILED_TO_CONNECT 11054
#define CLOSE_VPN_CLIENT 11055
#define INSTALL_TAP_DEVICE 10056
#define RECEIVE_TAP_COUNT 10057
#define CONNECT_TO_SERVICE 10058



// Magic number for the arrow in window position calculation
#define ARROW_HEIGHT 28


// Pragma to use SHAppBarMessage
#pragma comment (lib, "shell32.lib")

FrmMain*FrmMain::mInst = 0;

// Handle first tap count
static bool received_first_tap_count = false;
static std::mutex mutex_first_tap_count;
static std::condition_variable cv_first_tap_count;

// Class to work with database from Version 1
// We probably want to move that somewhere more reasonable
class OldDatabase
{
private:
     QSqlDatabase db;

public:
    OldDatabase(QString file)
    {
        if (!QSqlDatabase::database(QLatin1String("oldConnection")).isValid()) {
            db = QSqlDatabase::addDatabase("QSQLITE", QLatin1String("oldConnection"));
        } else {
            db = QSqlDatabase::database(QLatin1String("oldConnection"));
        }

         QString databaseFullPath (file);

         // Set database path
         this->db.setDatabaseName(databaseFullPath);
         // Init database
         this->db.setConnectOptions("QSQLITE_BUSY_TIMEOUT=50");
         // Open Databse
         if (!this->open()) {
             //
             QCoreApplication::exit(1);
         }
    }

    ~OldDatabase()
    {
        close();
    }

    bool open()
    {
        if (!this->db.open()) {
            return false;
        }

        return true;
    }

    void close()
    {
        if (this->db.isOpen()) {
            this->db.close();
        }

        this->db.removeDatabase(QLatin1String("oldConnection"));
    }

    bool execute(const QString &sql)
    {
        bool retVal (true);
        QSqlQuery *query = new QSqlQuery (QSqlDatabase::database(QLatin1String("oldConnection")));
        query->exec(sql);
        if (query->lastError().type() != QSqlError::NoError) {
        }

        query->clear();
        delete query;
        query = 0;

        return retVal;
    }

    QSqlQuery* openQuery(const QString &sql)
    {
        //
        // Open a new query and retur the recordset
        //

        // Build new query
        // The ownership changes to the caller
        QSqlQuery *query = new QSqlQuery (QSqlDatabase::database(QLatin1String("oldConnection")));
        // Exec query
        query->exec(sql);

        // Error occurred?
        if (query->lastError().type() != QSqlError::NoError) {
        }

        return query;
    }
};

#include <message.h>
#include <config/configvalues.h>
#include <qsettings.h>

HWND mainHWND;

bool FrmMain::instanceCheck()
{
    return mInst != nullptr;
}

FrmMain *FrmMain::instance()
{
    //
    // Create singleton
    //


    if (!mInst) {
        // Create a new instance
        mInst = new FrmMain;

        //activate debug
/*
        // Check if disable run file is available
        QFile disableDebugFile (qApp->applicationDirPath() + QLatin1String("/debug.disable.run.override"));
        if (!disableDebugFile.exists()) {
            qDebug() << "debug output check is enabled";

            // enable Debug
            Debug::enableDebugging(true);
            // set debug path to debug-vpn-client.txt
            Debug::setDebugPath(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
            //anable date and time stamp
            Debug::enableMSecs(true);

        }
*/

        // load settings from client.ini
        QSettings clientSettings (QCoreApplication::applicationDirPath() + QLatin1String("/client.ini"), QSettings::IniFormat);

        // Set a custom window text for SendMessage in combination with FindWindow
        SetWindowText((HWND)mInst->winId(), _TEXT("HalloSPSSLVPN"));
        mainHWND = (HWND)mInst->winId();
        /*for(const auto con : Configs::instance()->getList())
        {
            SrvCLI::instance()->send(QLatin1String("STATUS"), QString("%1").arg(con.second->GetId()));
        }*/

        //check if there is a config available
        if(!Configs::instance()->isConnectionAvailable()){
            mInst->checkForNewConfigAndRefreshUI();
        }

        Configs::instance()->refreshConfigs();

        mInst->updateUITimer = new QTimer(mInst);
        connect(mInst->updateUITimer, SIGNAL(timeout()), mInst, SLOT(refreshUI()));
        mInst->updateUITimer->start(24);
    }

    //
    return mInst;
}

#include <thread>


// This function runs in a thread and checks for changes in the taskbar
// Moving or resizing, so the main window can move accordingly
void TBThread()
{
    //
    // Check if the taskbar moved
    //

    ;
    while(true) {
        //
        Sleep(50);
        static RECT oldGeomentry;
        static bool oldGeomentryIsInitialized = false;

        // Get Taskbar Handle
        HWND hTaskBar = FindWindow(_TEXT("Shell_TrayWnd"), _TEXT(""));

        // Init the cache of the taskbar position
        if(!oldGeomentryIsInitialized) {
            // Get the current geomentry
            GetWindowRect(hTaskBar, &oldGeomentry);
            oldGeomentryIsInitialized = true;
        }

        RECT currentGeomentry;

        // Read the current geomentry of the taskbar
        GetWindowRect(hTaskBar, &currentGeomentry);

        // Check if somethind has changed
        if(memcmp(&oldGeomentry, &currentGeomentry, sizeof(RECT))) {
            // Yes, taskbar was moved or changed their height/ width
            SendMessage(mainHWND, ON_TB_MOVE, NULL, NULL);
            // Update the cache
            oldGeomentry = currentGeomentry;
        }
    }
}

WNDPROC pfOriginalProc;

bool bAllowMove = false;

bool FrmMain::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    MSG *winMessage = (MSG*) message;


    switch (winMessage->message)
    {
        case static_cast<UINT>(WM_POWERBROADCAST):
        {
            if(winMessage->wParam == PBT_APMSUSPEND
            || winMessage->wParam == PBT_APMSTANDBY)
            {
                for(const auto &conDis : Configs::instance()->getList())
                {
                    conDis.second->Disconnect();
                }
            }
            break;
        }
        case static_cast<UINT>(CLOSE_VPN_CLIENT):
        {
            //HWND senderHandle = (HWND) winMessage->hwnd;
            FrmMain::instance()->closeApplication();
            break;
        }
        case static_cast<UINT>(ON_TB_MOVE):
        {
           // HWND senderHandle = (HWND) winMessage->hwnd;
            FrmMain::instance()->taskBarMove();
            break;
        }
        case static_cast<UINT>(ON_SEND_DUMMY):
        {
            //HWND senderHandle = (HWND) winMessage->hwnd;
            SrvCLI::instance()->send(QLatin1String("Dummy"), g_strClientName);
            //if(qApp->hasPendingEvents())
            //{
                qApp->processEvents();
            //}
            break;
        }
        case static_cast<UINT>(FAILED_TO_CONNECT):
        {
           // HWND senderHandle = (HWND) winMessage->hwnd;
            Message::error(QObject::tr("Failed to connect to service! Please make sure the service is running and restart the application!"));
        break;
        }
        case static_cast<UINT>(INSTALL_TAP_DEVICE):
        {
            //HWND senderHandle = (HWND) winMessage->hwnd;
            SrvCLI::instance()->send(QLatin1String("ADDTAP"), QLatin1String(""));
            break;
        }
        case static_cast<UINT>(RECEIVE_TAP_COUNT):
        {
           // HWND senderHandle = (HWND) winMessage->hwnd;
            SrvCLI::instance()->send(QLatin1String("TAPCOUNT"), QLatin1String(""));
            break;
        }
        case static_cast<UINT>(CONNECT_TO_SERVICE):
        {
            //HWND senderHandle = (HWND) winMessage->hwnd;
            FrmMain::instance()->connectToService();
            break;
        }
        case static_cast<UINT>(WM_NCHITTEST):
        {


            LRESULT hit = DefWindowProc(winMessage->hwnd, winMessage->message, (int) winMessage->wParam, (int) winMessage->lParam);

            // check if moving is allowed
            // If not we always return hit client and not caption

            if(!bAllowMove) {
                return QWidget::nativeEvent(eventType, message, result);
            }

            if (hit == HTCLIENT) {
                // Cehck if the hit was in the title bar
                POINT p;

                if (GetCursorPos(&p)) {

                    // Get the client coordinate from the cursor position
                    if (ScreenToClient(winMessage->hwnd, &p))
                    {
                        if (p.y < 20) {
                            // Hit was in the title bar
                            *result = HTCAPTION;
                        }
                        else
                        {
                           return false;
                        }

                        RECT rect;
                        GetWindowRect(winMessage->hwnd, &rect);

                        // Check if the hit was on close button
                        if(p.x > ((rect.right - rect.left) - 25)) {
                            // Hit was on X button, so execute original wndProc

                            return QWidget::nativeEvent(eventType, message, result);
                        }
                    }
                }
            }
            return true;
        }
        default:
        {
            return QWidget::nativeEvent(eventType, message, result);
        }
    }
    return 0;
}

// This handles some custom Window Messages so we can send commands from any thread and they will be executed on the main thread
// QT requires that for many things
LRESULT wndproc1(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
        case WM_POWERBROADCAST:
        {
            if(wParam == PBT_APMSUSPEND
                || wParam == PBT_APMSTANDBY)
            {
                for(const auto &conDis : Configs::instance()->getList())
                {
                    conDis.second->Disconnect();
                }
            }
            break;
        }
        case CLOSE_VPN_CLIENT:
        {
            FrmMain::instance()->closeApplication();
            break;
        }
        case ON_TB_MOVE:
            FrmMain::instance()->taskBarMove();
            break;
        case ON_SEND_DUMMY:
            {
             SrvCLI::instance()->send(QLatin1String("Dummy"), g_strClientName);
             //(qApp->hasPendingEvents()) obsolet
                 qApp->processEvents();
            }
            break;
        case FAILED_TO_CONNECT:
            {
                Message::error(QObject::tr("Failed to connect to service! Please make sure the service is running and restart the application!"));
            }
            break;
        case INSTALL_TAP_DEVICE:
            {
                SrvCLI::instance()->send(QLatin1String("ADDTAP"), QLatin1String(""));
            }
            break;
        case RECEIVE_TAP_COUNT:
            {
                SrvCLI::instance()->send(QLatin1String("TAPCOUNT"), QLatin1String(""));
            }
            break;
        case CONNECT_TO_SERVICE:
            {
                FrmMain::instance()->connectToService();
            }
            break;
       case WM_NCHITTEST: {
            LRESULT hit = DefWindowProc(hwnd, uMsg, wParam, lParam);

            // check if moving is allowed
            // If not we always return hit client and not caption
            if(!bAllowMove) {
                return HTCLIENT;
            }

            if (hit == HTCLIENT) {
                // Cehck if the hit was in the title bar
                POINT p;
                if (GetCursorPos(&p)) {
                    // Get the client coordinate from the cursor position
                    if (ScreenToClient(hwnd, &p)) {
                        if (p.y < 20) {
                            // Hit was in the title bar
                            hit = HTCAPTION;
                        }

                        RECT rect;
                        GetWindowRect(hwnd, &rect);

                        // Check if the hit was on close button
                        if(p.x > ((rect.right - rect.left) - 25)) {
                            // Hit was on X button, so execute original wndProc
                            return CallWindowProc(pfOriginalProc, hwnd, uMsg, wParam, lParam);
                        }
                    }
                }
            }

            return hit;
        }
        default:
            return CallWindowProc(pfOriginalProc, hwnd, uMsg, wParam, lParam);
    }

    return 0;
}




FrmMain::FrmMain()
    : isReconnect(false),
      tapCount(0),
      installingTap(false),
      version("2.0.43"),
      ui(new Ui::FrmMain),
      qCurrentArrow(nullptr),
      widgetFactory(new WidgetFactory),
      updateUITimer(nullptr)
{
    ui->setupUi(this);

    // Hide unuses update buttons
    ui->cmdUpdateState->setVisible(false);


    QFont font("Verdana", 5, QFont::Normal);
    //font.setPixelSize(7);
    setFont(font);
    //
    // Init dialog
    //

    // No dialog border and no window title, also not shown in taskbar
    this->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    // Set windows transparent
    this->setAttribute(Qt::WA_TranslucentBackground, true);


    //ui->frameMainWindow->setLayout(ui->mainGrid);
    auto geom = ui->frameMainWindow->geometry();
   // geom.adjust(0, 0, 100, 100);
    ui->frameMainWindow->setGeometry(geom);

    geom = geometry();
    //geom.adjust(0, 0, 100, 100);
    setGeometry(geom);

    ui->widget->setLayout(ui->hHeader);

    ui->mainGrid->insertWidget(1, this->widgetFactory);


    // Check if the taskbar propertie autohide in enabled
    // If yes the windows is open centered
    this->checkTaskBarAutoHideProperty();

    // Init systray icon
    //
    // Create new object
    trayIcon = new QSystemTrayIcon(this);

    trayIcon->setToolTip("Securepoint SSL VPN");

    // Bind activate signal, this could be a double click, etc
    QObject::connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayActivated(QSystemTrayIcon::ActivationReason)));

    // Set the default icon
    QIcon icon = QIcon(":/data/images/state/lock_white.png");
    trayIcon->setIcon(icon);

    //
    //pfOriginalProc = (WNDPROC)SetWindowLong((HWND)winId(), GWL_WNDPROC, (long)wndproc1);

    // Start check taskbar geomentry thread
    std::thread th(TBThread);
    th.detach();

    // Calculate the correct size based on the scaling
    QSize arrowTargetSize(48 * windowsDpiScale(), 17 * windowsDpiScale());

    // Init the icon pointer arrows
    qArrowBottom = new QImage;
    qArrowBottom->load(":/data/images/arrow.png", "PNG");

    *qArrowBottom = qArrowBottom->scaled(arrowTargetSize);


    // Rotate the arrow image for all the directions it could have
    //
    qArrowTop = new QImage;
    qArrowTop->load(":/data/images/arrow.png", "PNG");
    QTransform trans;

    trans.rotate(180);
    *qArrowTop = qArrowTop->transformed(trans);
    *qArrowTop = qArrowTop->scaled(arrowTargetSize);
    //
    qArrowLeft= new QImage;
    qArrowLeft->load(":/data/images/arrow.png", "PNG");
    trans.rotate(270);
    *qArrowLeft = qArrowLeft->transformed(trans);
    *qArrowLeft = qArrowLeft->scaled(arrowTargetSize);
    //
    qArrowRight = new QImage;
    qArrowRight->load(":/data/images/arrow.png", "PNG");
    trans.rotate(180);
    *qArrowRight = qArrowRight->transformed(trans);
    *qArrowRight = qArrowRight->scaled(arrowTargetSize);

    //

    // The settings pushbutton menu
    RightAlignPushButtonMenu *menuSettings = new RightAlignPushButtonMenu (ui->cmdSettingMenu);
    // Set the object name for css
    menuSettings->setObjectName("settingsMenu");
    // Set the cursor
    menuSettings->setCursor(Qt::PointingHandCursor);

    // Add the actions
    if(Settings::instance()->managed()) {
        menuSettings->addAction(QIcon(":/data/images/new.png"), QObject::tr("New"), this, SLOT(showWizard()));
        menuSettings->addAction(QIcon(":/data/images/16_autostart.png"), QObject::tr("Import"), this->widgetFactory, SLOT(showImport()));
        menuSettings->addAction(QIcon(":/data/images/settings_03.png"), QObject::tr("Client settings"), this->widgetFactory, SLOT(showClientSettings()));
    }

    menuSettings->addAction(QIcon(":/data/images/information.png"), QObject::tr("Information"), this->widgetFactory, SLOT(showInformation()));

    // Assign menu to the pushbutton
    ui->cmdSettingMenu->setMenu(menuSettings);

    // Create menu
    QMenu *menu = new QMenu(this);
    //
    menu->setObjectName(QLatin1String("trayIconMenu"));
    menu->setCursor(Qt::PointingHandCursor);
    //
    menu->addAction(QObject::tr("Hide this menu"), menu, SLOT(hide()));
    menu->addSeparator();
    menu->addAction(QObject::tr("Show window"), this, SLOT(showHideTrayMenu()));

    menu->addSeparator();
    menu->addAction(QObject::tr("Quit"), this, SLOT(closeApplication()));

    this->trayIcon->setContextMenu(menu);

    this->widgetFactory->widget(MainView);
    // Display into systray

    qApp->installEventFilter(this);

    // Seems Qt fails :)

    //SetWindowPos((HWND)(reinterpret_cast<QWindow>(this))->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    //SetWindowPos((HWND)this->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    ui->frameMainWindow->setLayout(ui->mainGrid);
    geom = ui->frameMainWindow->geometry();
    auto size = geom.size() * windowsDpiScale();
    geom.setSize(size);
    ui->frameMainWindow->setGeometry(geom);

    geom = geometry();
    size = geom.size() * windowsDpiScale();
    geom.setSize(size);
    setGeometry(geom);

    static QImage img;

    // once again do some image scaling for the dpi scaling options
    static std::once_flag flag;
    std::call_once(flag, [this]() {
        img.load(":/data/images/logo.png", "PNG");
        img = img.scaled(QSize(122*windowsDpiScale(), 9*windowsDpiScale()), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    });

    ui->windowTitleIcon->setPixmap(QPixmap::fromImage(img));
    ui->windowTitleIcon->setMaximumSize(130*windowsDpiScale(), 16*windowsDpiScale());
    ui->windowTitleIcon->setMinimumSize(130*windowsDpiScale(), 16*windowsDpiScale());
    ui->windowTitleIcon->setFixedSize(130*windowsDpiScale(), 16*windowsDpiScale());
    auto g = ui->windowTitleIcon->geometry();
    g.setSize(QSize(130*windowsDpiScale(), 16*windowsDpiScale()));
    ui->windowTitleIcon->setGeometry(g);

    auto cmdClose_2 =  ui->cmdCloseWindow;
    cmdClose_2->setObjectName(QStringLiteral("close_dat_shit"));
    cmdClose_2->setGeometry(QRect(size.width(), 2, 16*windowsDpiScale(), 16*windowsDpiScale()));

    {
        QImage img(":/data/images/import.png");
        img = img.scaled(QSize(16*windowsDpiScale(), 16*windowsDpiScale()), Qt::KeepAspectRatio, Qt::SmoothTransformation);

        QIcon icon1;
        icon1.addPixmap(QPixmap::fromImage(img));

        cmdClose_2->setIcon(icon1);
        cmdClose_2->setIconSize(QSize(16*windowsDpiScale(), 16*windowsDpiScale()));
        cmdClose_2->setFlat(true);
    }

    // Load and migrate the proxy stuff if available
    ProxySettings::instance();
    // Start the update check
}

void FrmMain::refreshUI()
{
    QCoreApplication::processEvents();

    QApplication::processEvents();

    ((MainListView*)widgetFactory->widget(MainView))->update();
    ((MainListView*)widgetFactory->widget(MainView))->repaint();    

    { /* Do the resizing for the Settings Icon */
        static QImage img;

        static std::once_flag flag;
        std::call_once(flag, [this]() {
            img.load(":/data/images/wheel-dark.png", "PNG");
            img = img.scaled(QSize(16*windowsDpiScale(), 16*windowsDpiScale()), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        });

        ui->cmdSettingMenu->setIcon(QPixmap::fromImage(img));
        ui->cmdSettingMenu->setIconSize(img.size());
        ui->cmdSettingMenu->setMaximumSize(28*windowsDpiScale(), 20*windowsDpiScale());
        ui->cmdSettingMenu->setMinimumSize(28*windowsDpiScale(), 20*windowsDpiScale());
        ui->cmdSettingMenu->setFixedSize(28*windowsDpiScale(), 20*windowsDpiScale());
        auto g = ui->cmdSettingMenu->geometry();
        g.setSize(QSize(16*windowsDpiScale(), 20*windowsDpiScale()));
        ui->cmdSettingMenu->setGeometry(g);
    }

    QCoreApplication::processEvents();

    QApplication::processEvents();
}

void FrmMain::checkTaskBarAutoHideProperty()
{
    //
    // Check the auto hide property of the taskbar
    //

    APPBARDATA appBarData;
    appBarData.cbSize = sizeof(appBarData);
    UINT ucState = (UINT)SHAppBarMessage(ABM_GETSTATE, &appBarData);
    if(ucState) {
        this->m_bTaskBarAutoHide = (ABS_AUTOHIDE == ucState);
    } else {
        this->m_bTaskBarAutoHide = false;
    }
}

void FrmMain::sendClose()
{
    PostMessage(mainHWND, CLOSE_VPN_CLIENT, NULL, NULL);
}

std::condition_variable cvStartDaemon;
std::mutex mutexStartDaemon;

bool connectedToService = false;

void FrmMain::closeApplication()
{
    if(Settings::instance()->blockShutdown()) {
        // Check for shutdown, so ask to close connections, if connected
        for(const auto &con : Configs::instance()->getList()) {
            if(con.second->GetState() == ConnectionState::Connected) {
                if(Message::confirm(QObject::tr("You are currently connected. Do you want to close the connection(s)?"), QObject::tr("Please confirm"), true)) {
                    for(const auto &conDis : Configs::instance()->getList()) {
                        conDis.second->Disconnect();
                    }
                    //
                    break;
                } else {
                    return;
                }
            }
        }
    }

    //
    // Exit the vpn client
    //
    this->trayIcon->hide();

    // Fullfil all waiting conditions
    cvStartDaemon.notify_all();

    //
    QCoreApplication::processEvents();
    //
    QCoreApplication::exit();
}

FrmMain::~FrmMain()
{
    this->trayIcon->hide();
    //
    delete this->trayIcon;
    this->trayIcon = nullptr;

    // TODO: delet images

    delete qArrowBottom;
    delete qArrowLeft;
    delete qArrowRight;
    delete qArrowTop;

    //
    delete ui;
}

int arrowX = 0;
int arrowY = 0;

void FrmMain::paintEvent(QPaintEvent* pPaintEvent)
{
    //
    // Draw the icon pointing arrow
    //

    QPainter qpainter(this);
    // Makes sure that we have a valid pointer
    if(qCurrentArrow) {
        qpainter.drawImage(arrowX, arrowY, *qCurrentArrow);
    }

    // Call origin event
    QMainWindow::paintEvent(pPaintEvent);
}

enum Orientation
{
    BOTTOM,
    TOP,
    LEFT,
    RIGHT,
    CENTER,
};

// Default orientation is bottom
Orientation orientation = Orientation::BOTTOM;

void FrmMain::taskBarMove()
{
    //
    // If the taskbar moved and the clint window is visible
    // adjust arrow and window
    //

    if(this->isVisible()) {
        showEvent(0);
    }
}

WidgetFactory *FrmMain::mainWidget()
{
    //
    // Returns the main widget
    //

    return this->widgetFactory;
}

void FrmMain::showTrayIcon()
{
    if(!g_bSilent)
        this->trayIcon->show();
    else
        this->trayIcon->hide();
}

void FrmMain::setMenuToShowWindow()
{
    //
    // Reset menu text, due to speed up double click stuff in main.c
    // Show event will set the text to hide menu and the close in main
    // didn't fix that mess. So we need to do it by hand.
    //

    if (this->trayIcon->contextMenu()->actions().size() > 2) {
        this->trayIcon->contextMenu()->actions().at(2)->setText(QObject::tr("Show window"));
    }
}

static int prevX = -1;
static int prevY = -1;
static bool bSavePos = false;

enum class TaskBarEdge {

    LEFT = 0,
    TOP = 1,
    RIGHT = 2,
    BOTTOM = 3,
};

void FrmMain::showEvent(QShowEvent *event)
{
    //
    // Set window position
    //
    this->trayIcon->contextMenu()->actions().at(2)->setText(QObject::tr("Hide window"));

    Configs::instance()->refreshConfigs();

    ((MainListView*)this->widgetFactory->widget(MainView))->model.LoadConnections();

    // Get the taskbar auto hide value
    this->checkTaskBarAutoHideProperty();

    QScreen  *desktop = qApp->primaryScreen();

    // First we try only single screens
    //int desktopWidth (desktop->screenGeometry(desktop->screenNumber(QCursor::pos())).width());
      int desktopWidth (desktop->geometry().width());

    //auto screenGeometry = desktop->screenGeometry(desktop->screenNumber(QCursor::pos()));
      auto screenGeometry = desktop->geometry();

    // Get the icon postion
    QPoint trayIconRightTopCorner (this->trayIcon->geometry().topLeft());
    // Determine the space between the systray icon and the right screen border
    //int freeHorizontalSpace (desktopWidth - (trayIconRightTopCorner.x() - desktop->screenGeometry(desktop->screenNumber(QCursor::pos())).left()));
      int freeHorizontalSpace (desktopWidth - (trayIconRightTopCorner.x() - desktop->geometry().left()));


    int positionIconHorizontalSpace (28);
    int positionIconVerticalSpace (0);


    HWND hTaskBar = FindWindow(_TEXT("Shell_TrayWnd"), _TEXT(""));

#if 1
    // Possible fix for Windows 8 Multiple Monitors
    do
    {
        RECT tbRect;
        if(!hTaskBar)
            break;


        GetWindowRect(hTaskBar, &tbRect);

        // center of taskbar
        QPoint tbPoint(tbRect.left + ((tbRect.right - tbRect.left) / 2), tbRect.top + ((tbRect.bottom - tbRect.top) / 2));

        // Check if the point is in the screen
        //if(desktop->screenGeometry(desktop->screenNumber(QCursor::pos())).contains(tbPoint))
          if(desktop->geometry().contains(tbPoint))
            break;
    }

    while(hTaskBar = FindWindowEx(NULL, hTaskBar, _TEXT("Shell_TrayWnd"), NULL));
    {
        RECT tbRect;

        if(hTaskBar)
        {

            GetWindowRect(hTaskBar, &tbRect);

            // center of taskbar
            QPoint tbPoint(tbRect.left + ((tbRect.right - tbRect.left) / 2), tbRect.top + ((tbRect.bottom - tbRect.top) / 2));
            //if(!desktop->screenGeometry(desktop->screenNumber(QCursor::pos())).contains(tbPoint))
            if(!desktop->geometry().contains(tbPoint))
            {
                hTaskBar = NULL;
            }
        }
        /// In case we still have no TrayWnd try to use SecondayTrayWnd
        if(!hTaskBar)
        {
            hTaskBar = FindWindow(_TEXT("Shell_SecondaryTrayWnd"), _TEXT(""));
            do
            {
                RECT tbRect;

                if(!hTaskBar)
                    break;

                GetWindowRect(hTaskBar, &tbRect);

                // center of taskbar
                QPoint tbPoint(tbRect.left + ((tbRect.right - tbRect.left) / 2), tbRect.top + ((tbRect.bottom - tbRect.top) / 2));

                // Check if the point is in the screen
                //if(desktop->screenGeometry(desktop->screenNumber(QCursor::pos())).contains(tbPoint))
                  if(desktop->geometry().contains(tbPoint))
                    break;
            }
            while(hTaskBar = FindWindowEx(NULL, hTaskBar, _TEXT("Shell_SecondaryTrayWnd"), NULL));
            {
                GetWindowRect(hTaskBar, &tbRect);

                // center of taskbar
                QPoint tbPoint(tbRect.left + ((tbRect.right - tbRect.left) / 2), tbRect.top + ((tbRect.bottom - tbRect.top) / 2));

                /// In case we still have no TrayWnd try to use SecondayTrayWnd
                //if(!desktop->screenGeometry(desktop->screenNumber(QCursor::pos())).contains(tbPoint))
                  if(!desktop->geometry().contains(tbPoint))
                {
                    hTaskBar = 0;
                }
            }
        }
    }

    // If we still have no hTaskBar then we are fucked and cant do anything,
    // We should open the window in the center of the main monitor
#endif

    HWND hNotifyArea = FindWindowEx(NULL, NULL, _TEXT("NotifyIconOverflowWindow"), _TEXT(""));

    static int origW = this->width();
    static int origH = this->height();


    int oriX (0);
    int oriY (0);

    int frameX (0);
    int frameY (0);

    APPBARDATA pabd;

    pabd.hWnd = hTaskBar;
    pabd.cbSize = sizeof(APPBARDATA);

    SHAppBarMessage(ABM_GETTASKBARPOS, &pabd);

    TaskBarEdge tbPos = (TaskBarEdge)pabd.uEdge;

    if(IsWindowVisible(hNotifyArea) || m_bTaskBarAutoHide || !hTaskBar) {
        RECT rect;
        GetWindowRect(hNotifyArea, &rect);

        if((this->trayIcon->geometry().topLeft().y() >= rect.top && this->trayIcon->geometry().topLeft().y() <= rect.bottom) || m_bTaskBarAutoHide || !hTaskBar) {
            qCurrentArrow = nullptr;

            bSavePos = true;
            bAllowMove = true;

            if(prevX == -1 && prevY == -1)
            {
                positionIconHorizontalSpace = (screenGeometry.width() / 2 - this->width() / 2);
                positionIconVerticalSpace = screenGeometry.height() / 2 - this->height() / 2;
            }
            else if(this->isVisible())
            {
                positionIconHorizontalSpace = this->geometry().topLeft().x();
                positionIconVerticalSpace = this->geometry().topLeft().y();
            }
            else
            {
                positionIconHorizontalSpace = prevX;
                positionIconVerticalSpace = prevY;
            }

            //
            this->setGeometry(positionIconHorizontalSpace, positionIconVerticalSpace, origW, origH);
            ui->frameMainWindow->setGeometry(0, 0, ui->frameMainWindow->width(), ui->frameMainWindow->height());

            orientation = Orientation::CENTER;
        } else {
            this->setGeometry(trayIconRightTopCorner.x() - positionIconHorizontalSpace, trayIconRightTopCorner.y() - ui->frameMainWindow->height() - positionIconVerticalSpace, this->width(), this->height());
        }
    } else {
        bSavePos = false;
        bAllowMove = false;
        RECT rect;
        GetWindowRect(hTaskBar, &rect);

        if(tbPos == TaskBarEdge::LEFT) {
            orientation = Orientation::LEFT;
            qCurrentArrow = qArrowLeft;
            oriX = qCurrentArrow->width();

            positionIconHorizontalSpace = -(rect.right - trayIconRightTopCorner.x()) + 2;

            positionIconVerticalSpace = ui->frameMainWindow->height() - qCurrentArrow->size().height()*2; // Change to substract something from desktop height?!

            arrowX = 1;
            arrowY = trayIconRightTopCorner.y() - qCurrentArrow->size().height() / 4;

            frameX = qCurrentArrow->width();
            frameY = 0;

        } else if(tbPos == TaskBarEdge::BOTTOM) {
            // TaskBar is bottom
            orientation = Orientation::BOTTOM;
            qCurrentArrow = qArrowBottom;

            oriY = qCurrentArrow->height();

            // Did the window fit completly
            positionIconVerticalSpace = (trayIconRightTopCorner.y() - rect.top) + ui->frameMainWindow->height() + qCurrentArrow->height() - 6;

            if (freeHorizontalSpace > origW) {
                // Set left corner of the dialog above the icon
                positionIconHorizontalSpace = qCurrentArrow->size().height();
            } else if (freeHorizontalSpace > ((origW) / 2) + qCurrentArrow->size().height() /* yepp its a magic number */) {
                // Center the dialog above the icon
                positionIconHorizontalSpace = origW / 2;
            } else {
                // Too few free space left, place the windows right corner to the tray icon
                positionIconHorizontalSpace = origW - freeHorizontalSpace + qCurrentArrow->size().height();
            }

            arrowY = rect.top - qCurrentArrow->size().height();
            arrowX = (trayIconRightTopCorner.x() - (trayIconRightTopCorner.x() - positionIconHorizontalSpace)) - qCurrentArrow->size().width() / 4;

            frameX = 0;
            frameY = 0;
        } else if(tbPos == TaskBarEdge::TOP) {
            positionIconVerticalSpace = -(rect.bottom - rect.top) + 1;

            orientation = Orientation::TOP;
            qCurrentArrow = qArrowTop;

            if (freeHorizontalSpace > origW) {
                // Set left corner of the dialog above the icon
                positionIconHorizontalSpace = qCurrentArrow->size().height();
            } else if (freeHorizontalSpace > (origW / 2) + qCurrentArrow->size().height() /* yepp its a magic number */) {
                // Center the dialog above the icon
                positionIconHorizontalSpace = origW / 2;
            } else {
                // Too few free space left, place the windows right corner to the tray icon
                positionIconHorizontalSpace = origW - freeHorizontalSpace + ARROW_HEIGHT;

            }

            oriY = qCurrentArrow->height();

            frameX = 0;
            frameY = qCurrentArrow->height();

            arrowX = (trayIconRightTopCorner.x() - (trayIconRightTopCorner.x() - positionIconHorizontalSpace)) - qCurrentArrow->size().width() / 4;
            arrowY = 1;
        } else {
            orientation = Orientation::RIGHT;
            qCurrentArrow = qArrowRight;

            oriX = qCurrentArrow->width();

            positionIconHorizontalSpace = -(rect.left - trayIconRightTopCorner.x() - (origW + oriX)) - 5;

            positionIconVerticalSpace = ui->frameMainWindow->height() - qCurrentArrow->size().height()*2;


            arrowY = trayIconRightTopCorner.y() - qCurrentArrow->size().height() / 4;
            arrowX = rect.left - qArrowRight->width();

            frameX = 0;
            frameY = 0;
        }

        // Set the new geomentry of the dialog
        this->setGeometry(trayIconRightTopCorner.x() - positionIconHorizontalSpace, trayIconRightTopCorner.y() - positionIconVerticalSpace, origW + oriX, origH + oriY);

        ui->frameMainWindow->setGeometry(frameX, frameY, ui->frameMainWindow->width(), ui->frameMainWindow->height());
    }

    if(orientation == Orientation::BOTTOM || orientation == Orientation::LEFT || orientation == Orientation::RIGHT) {
        POINT point;
        point.x = 0;
        point.y = arrowY;

        ScreenToClient((HWND)mInst->winId(), &point);

        arrowY = point.y;
    }

    if(orientation == Orientation::TOP) {
        POINT point;
        point.x = arrowX;
        point.y = 0;
        ScreenToClient((HWND)mInst->winId(), &point);

        arrowX = point.x;
    }

    // Show and activate window
    this->show();

    if(event) {
        this->activateWindow();
    }

    this->repaint();
    // All ok
    if(event) {
        event->accept();
    }

    if(orientation == Orientation::CENTER)
    {
        SetWindowPos((HWND)this->winId(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
        SetWindowPos((HWND)this->winId(), HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
    }
    else
        SetWindowPos((HWND)this->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}



bool FrmMain::initDaemon()
{
    QObject::connect(SrvCLI::instance(), SIGNAL(setDisconnected(int)), this, SLOT(setDisconnected(int)));
    // Fehler vom Service
    QObject::connect(SrvCLI::instance(), SIGNAL(setError(int,QString)), this, SLOT(setError(int,QString)));
    // User eingabe wird erwartet
    QObject::connect(SrvCLI::instance(), SIGNAL(needUserInput(int,int)), this, SLOT(userInputIsNeeded(int,int)));
    // Connection steht
    QObject::connect(SrvCLI::instance(), SIGNAL(receivedIP(int,QString)), this, SLOT(receivedIP(int,QString)));
    // Haben einen Reconnect bekommen
    QObject::connect(SrvCLI::instance(), SIGNAL(receivedReconnect(int)), this, SLOT(receivedReconnect(int)));
    // Tap Steuerung
    QObject::connect(SrvCLI::instance(), SIGNAL(receivedTapControl(int)), this, SLOT(receivedTapControl(int)));
    QObject::connect(SrvCLI::instance(), SIGNAL(receivedRemoveTap(QString)), this, SLOT(receivedRemoveTap(QString)));
    QObject::connect(SrvCLI::instance(), SIGNAL(receivedTapCount(int)), this, SLOT(receivedTapCount(int)));
    // Status
    QObject::connect(SrvCLI::instance(), SIGNAL(receivedStatus(int,bool,bool,int,QString)), this, SLOT(receivedStatus(int,bool,bool,int,QString)));

    QObject::connect(SrvCLI::instance(), SIGNAL(receivedDummy(quint32)), this, SLOT(dummyReceived(quint32)));

    // Check for updates
    UpdateCheck *doUpdateCheck = new UpdateCheck;
    QObject::connect(doUpdateCheck, SIGNAL(updateAvailable(QString)), this, SLOT(updateAvailable(QString)));
    // Fire
    doUpdateCheck->run();

    return true;
}


void FrmMain::connectToService()
{
    if(SrvCLI::instance()->makeFastConnection()) {
        cvStartDaemon.notify_all();
    }
}

void FrmMain::dummyReceived(quint32 id)
{
    Q_UNUSED(id)

    cvStartDaemon.notify_all();
}

bool FrmMain::startDaemon()
{
    // Nun die Verbindung testen
    int tries (0);
    bool success (false);

    do {
        // This extra scope is for the unique lock
        {
            if(!SrvCLI::instance()->isOnline()) {
                // Send connect to message queue
                PostMessage(mainHWND, CONNECT_TO_SERVICE, NULL, NULL);

                // Lock and wait for notify until timeout is reached
                std::unique_lock<std::mutex> lk(mutexStartDaemon);
                if(cvStartDaemon.wait_for(lk, std::chrono::milliseconds(2000)) == std::cv_status::no_timeout) {
                    // Timeout
                    success = true;
                }
            }
        }

        // If no error occurred we are connected
        // Send the dummy command to check if the
        // service is available
        if(success) {
            // Send dummy command to message queue
            SendMessage(mainHWND, ON_SEND_DUMMY, NULL, NULL);

            // Lock and wait for notify until timeout is reached
            std::unique_lock<std::mutex> lk(mutexStartDaemon);
            if (cvStartDaemon.wait_for(lk, std::chrono::milliseconds(2000)) == std::cv_status::no_timeout) {
                // All fine, exit loop
                qDebug() << QString("Service connection is stable");
                //
                break;
            }
        }

        // Check for events which needed to process
        //if (qApp->hasPendingEvents()) {
            qApp->processEvents();
        //}

        //
        qDebug() << QString(QString("Connect try: No.: %1 failed")
                           .arg(++tries));

        if(tries > 5) {
            // Okay thats it, bye bye
            SendMessage(mainHWND, FAILED_TO_CONNECT, NULL, NULL);
            //
            return false;
        }
    } while (true);

    connectedToService = true;

    // Get the tap count
    SendMessage(mainHWND, RECEIVE_TAP_COUNT, NULL, NULL);

    int autoStartCount = 0;
    // Get the number of tap devices we have to install
    for(const auto& con : Configs::instance()->getList()) {
        if(con.second->IsAutoStart()) {
            ++autoStartCount;
        }
    };

    // Wait for the tap count to be received
    {
        std::unique_lock<std::mutex> lk(mutex_first_tap_count);
        cv_first_tap_count.wait(lk, []{ return received_first_tap_count; } /* this is not necessary, but allows use with special condition */);
    }

    while(this->tapCount < autoStartCount) {
        // Set the bool for first tap count to false so the below code blocks and waits for the tap count to be received
        received_first_tap_count = false;
        SendMessage(mainHWND, INSTALL_TAP_DEVICE, NULL, NULL);

        // Wait for the tap count to be received
        std::unique_lock<std::mutex> lk(mutex_first_tap_count);
        cv_first_tap_count.wait(lk, []{ return received_first_tap_count; } /* this is not necessary, but allows use with special condition */);
    }

    // Now start all the autostart configs
    for(const auto& con : Configs::instance()->getList()) {
        if(con.second->IsAutoStart()) {
            con.second->Connect();

            // Wait so we dont spam and the service has enough time to initialize the connect so we dont try to connec on the same tap device
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
        }
    }

    return true;
}

void FrmMain::trayActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::DoubleClick) {
        if (!this->isVisible()) {
            // Make sure that the user seen the dh key warning
            this->showDiffieHellmanWarning();
            //
            this->showNormal();
            this->setFocus();
            this->activateWindow();
            //
            if (this->trayIcon->contextMenu()->actions().size() > 2) {
                this->trayIcon->contextMenu()->actions().at(2)->setText(QObject::tr("Hide window"));
            }
        } else {
            this->close();
            // Update menu action hide show window
            if (this->trayIcon->contextMenu()->actions().size() > 2) {
                this->trayIcon->contextMenu()->actions().at(2)->setText(QObject::tr("Show window"));
            }
        }
    }
}

void FrmMain::updateAvailable(QString version)
{
    this->updateVersion = version;


    if(version == "error"){
            ui->cmdUpdateState->setEnabled(false);
            ui->cmdUpdateState->setText(QObject::tr("Cannot check for Updates"));

    }
    else{
            ui->cmdUpdateState->setText(QObject::tr("%1 is available").arg(version));
    }

    ui->cmdUpdateState->setVisible(true);
}

void FrmMain::on_cmdCloseWindow_clicked()
{
    QWidget *tlw = window();
    mainHWND = (HWND)tlw->internalWinId();

    if (this->trayIcon->contextMenu()->actions().size() > 2) {
        this->trayIcon->contextMenu()->actions().at(2)->setText(QObject::tr("Show window"));
    }

    SendMessage(mainHWND, WM_CLOSE, NULL, NULL);
}

void FrmMain::on_cmdSettingMenu_clicked()
{
    this->close();
}

void FrmMain::showHideTrayMenu()
{
    if(this->isVisible()) {
        this->trayIcon->contextMenu()->actions().at(2)->setText(QObject::tr("Show window"));
        this->close();
    }
    else {
        // Make sure that the user seen the dh key warning
        this->showDiffieHellmanWarning();
        //
        this->show();
    }
}

extern bool g_bPortable;


void FrmMain::showDiffieHellmanWarning()
{
    //
    // Show a warning about to small dh keys, when we in portable mode
    // and the user opens the window the first time.
    //

    static bool calledTwice = false;


    // Show smal dh key warning
    if(!g_bSilent && Settings::instance()->dhShowSmallKeyInformation() && !calledTwice) {
        // Show the warning only once
        calledTwice = true;
        //
        static std::once_flag flag;
        std::call_once(flag, [this]() {
            // Show a information mesagebox with a checkbox "don't show me again"
            QMessageBox box(0);
            QCheckBox dontShowAgain (QObject::tr("Close and don't show again"), &box);

            box.setStyleSheet("QPushButton:hover {background-color: rgb(195, 195, 195);} QPushButton {;text-align:center;padding-right:8px; padding-left: 8px;padding-top: 8px;padding-bottom: 8px;}");
            box.setModal(true);
            box.setWindowFlags(Qt::WindowStaysOnTopHint);
            // Init box
            box.setStandardButtons(QMessageBox::Ok);
            box.setDefaultButton(QMessageBox::Ok);
            box.setIcon(QMessageBox::Information);
            box.setTextFormat(Qt::RichText);

            box.addButton(&dontShowAgain, QMessageBox::ActionRole);

            // Set data
            box.setWindowTitle(QObject::tr("Information"));
            box.setText(QObject::tr("This version needs at least a serverside 1024 bit dh key. If you get an error like 'dh key too small' please use the 2.0.15 client and contact your server administrator.\n<a href=\"https://support.securepoint.de/viewtopic.php?t=6216\">https://support.securepoint.de/viewtopic.php?t=6216</a>"));

            //
            box.exec();

            if (dontShowAgain.checkState() == Qt::Checked) {
                // Mark son't show again
                Settings::instance()->setDhShowSmallKeyInformation(false);
            }

        });
    }

    calledTwice = true;
}

void FrmMain::on_cmdUpdateState_clicked()
{
    // https://updatevpn.securepoint.de/sslvpn-client/${version}/VPNClientInstaller.zip -
    if (Message::confirm(QObject::tr("Do you want to download the installer of the new SSL VPN v%1").arg(this->updateVersion), QObject::tr("An Update is available"))) {
        QDesktopServices::openUrl(QUrl(QString("https://updatevpn.securepoint.de/sslvpn-client/%1/VPNClientInstaller.zip").arg(this->updateVersion)));
    }
}

void FrmMain::setDisconnected(int id)
{
    // Alle TreeItem durchsuchen bis die ID gefunden wurde
    auto pConnection = ((MainListView*)this->widgetFactory->widget(MainView))->model.GetConnection(id);

    if(pConnection)
    {
        if(pConnection->GetState() != ConnectionState::Error)
            pConnection->SetState(ConnectionState::Disconnected);
    }
}

void FrmMain::setError(int id, QString message)
{
    auto pConnection = ((MainListView*)this->widgetFactory->widget(MainView))->model.GetConnection(id);

    if(pConnection)
    {
        pConnection->SetState(ConnectionState::Error);
        pConnection->SetError(message);
    }

}

void FrmMain::receivedIP(int id, QString ip)
{
    auto pConnection = ((MainListView*)this->widgetFactory->widget(MainView))->model.GetConnection(id);

    if(pConnection) {
        pConnection->SetState(ConnectionState::Connected);
        pConnection->SetIP(ip);
        pConnection->SetLastConnected(QDateTime::currentDateTime().toSecsSinceEpoch());
    }
#if 0
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
#endif
}

void FrmMain::receivedReconnect(int id)
{
    auto pConnection = ((MainListView*)this->widgetFactory->widget(MainView))->model.GetConnection(id);

    if(pConnection)
        pConnection->SetState(ConnectionState::Connecting);
}

void FrmMain::receivedTapCount(int count)
{
    ((ClientSettingsWidget*)this->widgetFactory->widget(ClientSettings))->receivedTapCount(count);

    // Release the lock before returning so we can notify
    {
        std::lock_guard<std::mutex> lk(mutex_first_tap_count);
        this->tapCount = count;
        received_first_tap_count = true;
    }

    cv_first_tap_count.notify_all();
}

void FrmMain::receivedTapControl(int type)
{
    static std::vector<ConnectionData *> connections;
    // Tap soll installiert werden, wurde erfolgreich installiert oder
    // die Installation ist fehlgeschlagen
    if (type == 0) {
        QApplication::restoreOverrideCursor();

        Message::information(QObject::tr("TAP device installed successfully.\nPlease reconnect the connection."), QObject::tr("User information"), this);

        if(g_bSilent)
        {
            for(auto &con : connections)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                con->Connect();
                refreshUI();
            }
        }

        this->widgetFactory->widget(MainView)->setEnabled(true);
        SrvCLI::instance()->send(QLatin1String("TAPCOUNT"), QLatin1String(""));
        connections.clear();

        installingTap = false;

    } else if (type == -1) {
        QApplication::restoreOverrideCursor();
        Message::error(QObject::tr("TAP device installed failed!"), QObject::tr("An error occurred"), this);
        this->widgetFactory->widget(MainView)->setEnabled(true);
    } else if (type == 1) {
        // Neues Device
        printf("needed\n");

        if (Message::confirm(QObject::tr("A TAP device ist needed. Do you want to install a device? All active connections will be disconnected before installation. You have to manually reconnect them."), QObject::tr("Please confirm"), true)) {
            QApplication::setOverrideCursor(Qt::WaitCursor);

            for(auto &con : Configs::instance()->getList())
            {
                if(con.second->GetState() == ConnectionState::Connected
                    || con.second->GetState() == ConnectionState::Connecting)
                {
                    connections.push_back(con.second);
                    con.second->Disconnect();
                }
            }

            this->widgetFactory->widget(MainView)->setEnabled(false);

            SrvCLI::instance()->send(QLatin1String("ADDTAP"), QLatin1String(""));
        } else {
            QApplication::restoreOverrideCursor();
        }
    }
}

void FrmMain::receivedStatus(int id, bool isConnected, bool isConnecting, int lastAction, QString ip)
{
    auto pConnection = ((MainListView*)this->widgetFactory->widget(MainView))->model.GetConnection(id);

    if(pConnection) {
        if(isConnecting) {
            pConnection->SetState(ConnectionState::Connecting);
        } else if(isConnected) {
            pConnection->SetState(ConnectionState::Connected);
        }
        else
        {
            pConnection->SetState(ConnectionState::Disconnected);

            if(this->isReconnect)
                pConnection->Connect();
        }
        if (lastAction > -1) {
            // Eine benutzeraktion wird erwartet
            this->userInputIsNeeded(pConnection->GetId(), lastAction);
        }
    }

    this->refreshUI();
}



void FrmMain::userInputIsNeeded(int id, int type)
{

    // If username was given at command line parameter use it
    if(type == 0)
    {
        if(!Settings::instance()->startUser().isEmpty())
        {
            SrvCLI::instance()->send(QLatin1String("UNEEDED"), QString("%1;%2").arg(id).arg(Settings::instance()->startUser()));
            SrvCLI::instance()->send(QLatin1String("null"), QString("%1;%2").arg("null").arg("null"));
            return;
        }
    }

    if(type == 1)
    {
        if(!Settings::instance()->startPassword().isEmpty())
        {
            SrvCLI::instance()->send(QLatin1String("PWDNEEDED"), QString("%1;%2").arg(id).arg(Settings::instance()->startPassword()));
            SrvCLI::instance()->send(QLatin1String("null"), QString("%1;%2").arg("null").arg("null"));
            return;
        }
    }

    if(type == -1)
    {
        auto pConnection = ((MainListView*)this->widgetFactory->widget(MainView))->model.GetConnection(id);

        if(pConnection)
        {
            for(int i = 0; i < 7; ++i)
            {
                pConnection->saveUserData(id, i, "", true);
            }
            //
            QApplication::processEvents();

            ((MainListView*)widgetFactory->widget(MainView))->update();
            ((MainListView*)widgetFactory->widget(MainView))->repaint();
        }



        return;
    }

    InputType::UserInputType ntype;
    // 0 - Username
    // 1 - Pwd
    // 2 - OTP
    // 3 - PKCS12
    // 4 - Private Key f�r Crypted User Data
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
    auto pConnection = ((MainListView*)this->widgetFactory->widget(MainView))->model.GetConnection(id);

    if(pConnection)
    {
        cName = pConnection->GetName();
        if(pConnection->HasCrediantials(type)) {

            QString value (pConnection->getSavedUserData(type));

            SrvCLI::instance()->send(command, QString("%1;%2").arg(id).arg(value));
            //send empty command to clear memory
            QString randomString = Utils::GetRandomString(127);
            SrvCLI::instance()->send("null", QString("%1;%2").arg(randomString).arg(randomString));
            Crypt::clearEncryptedArray();

            return;
        }
    }

    // Es sind keine Daten gespeichert, neue abfragen
    FrmGetUserData *dialog = new  FrmGetUserData(ntype, cName, mainHWND, id);
    QObject::connect(dialog, SIGNAL(saveUserData(int, int, QString, bool)), this, SLOT(saveUserData(int, int, QString, bool)));
    dialog->setWindowFlags(dialog->windowFlags() | Qt::WindowStaysOnTopHint);
    dialog->setModal(true);
    dialog->setAttribute(Qt::WidgetAttribute::WA_DeleteOnClose);
    dialog->open();

    return;

}

void FrmMain::saveUserData(int id, int type, QString value, bool save)
{
    auto pConnection = ((MainListView*)this->widgetFactory->widget(MainView))->model.GetConnection(id);
    if(pConnection) {
        pConnection->saveUserData(id, type, value, save);
    }
}

void FrmMain::receivedRemoveTap(QString state)
{
    if (state == QLatin1String("OK")) {
        QApplication::restoreOverrideCursor();
        Message::information(QObject::tr("TAP devices remove successfully."), QObject::tr("User information"), this);

        SrvCLI::instance()->send(QLatin1String("TAPCOUNT"), QLatin1String(""));
    } else if (state == QLatin1String("ERROR")) {
        QApplication::restoreOverrideCursor();
        Message::error(QObject::tr("Unable to uninstall TAP-Win32 driver!\nMaybe you have no permissions.\nPlease contact your system administrator."), QObject::tr("Securepoint SSL VPN"), this);
    }
}

void FrmMain::checkForNewConfigAndRefreshUI()
{
    //
    // Check for new configs and refresh the model and ui
    //

    this->activateWindow();

    // Get all current configs
    Configs::instance()->findConfigsInDir((Utils::userApplicationDataDirectory() + "/config"));

    if(g_bPortable){      
        QDir dir(Utils::userApplicationDataDirectory());
        dir.cdUp();
        Configs::instance()->findConfigsInDir((dir.path() + "/data/config"));
    }

    // Update list
    Configs::instance()->refreshConfigs();
    // Repaint widgets
    this->refreshUI();

    // Load all connections
    ((MainListView*)this->widgetFactory->widget(MainView))->model.LoadConnections();
}

void FrmMain::showWizard()
{

    VpnWizard vpn;

    vpn.exec();

    // Check for new configs
    this->checkForNewConfigAndRefreshUI();
}

void FrmMain::setIcon()
{
    //
    // Die Connection liste durchlaufen und nach connectionstate das icon setzen
    //

    bool configError (false);
    bool configConnecting (false);
    bool configConnected (false);


    for(auto con : Configs::instance()->getList())
    {
        switch (con.second->GetState())
        {
        case ConnectionState::Connecting:
            configConnecting = true;
            break;
        case ConnectionState::Connected:
            configConnected = true;
            break;
        case ConnectionState::Error:
            configError = true;
            break;
        default:
            break;
            // Nix
        }
    }

    // Nun die Kobinationen auswerten
    QIcon icon (":/data/images/state/lock_white.png");

    if (configConnecting) {
        icon = QIcon(":/data/images/state/lock_yellow.png");
    }

    if (configConnected) {
        icon = QIcon(":/data/images/state/lock_green.png");
    }

    if (configError) {
        icon = QIcon(":/data/images/state/lock_red.png");
    }

    this->trayIcon->setIcon(icon);
}

void FrmMain::checkIfSendToTrayIsNeeded()
{
    // Send to tray after all connection configs succesfully connect
    // This method is called by the ConnectionData::startAfterConnectDelayed method after we received a ip from openvpn
    // with a min delay of 2000ms.
    if (Settings::instance()->sendToTrayAfterConnect()) {
        // Get the states
        bool configError (false);
        bool configConnecting (false);
        bool configConnected (false);

        for(auto con : Configs::instance()->getList())
        {
            switch (con.second->GetState())
            {
            case ConnectionState::Connecting:
                configConnecting = true;
                break;
            case ConnectionState::Connected:
                configConnected = true;
                break;
            case ConnectionState::Error:
                configError = true;
                break;
            default:
                break;
                // Nix
            }
        }

        //
        if (!configError && !configConnecting && configConnected) {
            if (this->isVisible()) {
                //
                this->trayIcon->contextMenu()->actions().at(2)->setText(QObject::tr("Show window"));
                //
                this->close();
            }
        }
    }
}


const float DEFAULT_DPI = 96.0;

float windowsDpiScale()
{
    //return 2;
    HDC screen = GetDC(0);
    float dpiX = static_cast<float>(GetDeviceCaps(screen, LOGPIXELSX));
    ReleaseDC(0, screen);
    //
    return (dpiX/DEFAULT_DPI);
}
