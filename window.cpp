
#include <QtGui>
#include "window.h"


Window::Window()
{

    AppFunc app;
    if (!app.isAppPortable())
        myConfigs.searchConfigs(QDir::homePath() + QString("/Securepoint/OpenVPN"));

    myConfigs.searchConfigs(QDir::currentPath());
    subMenuList = myConfigs.getConfigsObjects();

    createActions();
    createTrayIcon();

    // Icon setzen und anzeigen
    QIcon icon = QIcon(":/images/inaktiv.svg");
    trayIcon->setIcon(icon);
    trayIcon->show();

    setWindowTitle(tr("Systray"));
    resize(400, 300);
}

void Window::setVisible(bool visible)
{
    preferencesAction->setEnabled(isMaximized() || !visible);
    QDialog::setVisible(visible);
}

void Window::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible()) {
        hide();
        event->ignore();
    }
}

void Window::createActions()
{
    preferencesAction = new QAction(tr("&Manage Connections"), this);
    connect(preferencesAction, SIGNAL(triggered()), this, SLOT(manageConnections()));

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void Window::manageConnections () {
    mydia.openDialog(subMenuList);
}

void Window::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    if (subMenuList.size() == 1) {
        foreach (OpenVpn *configObj, subMenuList) {
            menuChildList.append(mySubAction = new QAction(tr ("Connect"), this));
            mySubAction->setObjectName(configObj->configName);
            connect(mySubAction, SIGNAL(triggered()), configObj, SLOT(openConnect()));
            trayIconMenu->addAction (mySubAction);

            menuChildList.append(mySubAction = new QAction(tr ("Disconnect"), this));
            mySubAction->setObjectName(configObj->configName);
            mySubAction->setDisabled(true);
            connect(mySubAction, SIGNAL(triggered()), configObj, SLOT(disconnectVpn()));
            trayIconMenu->addAction (mySubAction);

            trayIconMenu->addSeparator();

            menuChildList.append(mySubAction = new QAction(tr ("Show log"), this));
            mySubAction->setObjectName(configObj->configName);
            connect(mySubAction, SIGNAL(triggered()), configObj, SLOT(openVpnLog()));
            trayIconMenu->addAction (mySubAction);

            menuChildList.append(mySubAction = new QAction(tr ("Edit config"), this));
            mySubAction->setObjectName(configObj->configName);
            connect(mySubAction, SIGNAL(triggered()), configObj, SLOT(editVpnConfig()));
            trayIconMenu->addAction (mySubAction);

            menuChildList.append(mySubAction = new QAction(tr ("Change password"), this));
            mySubAction->setObjectName(configObj->configName);
            connect(mySubAction, SIGNAL(triggered()), configObj, SLOT(openConnect()));
            trayIconMenu->addAction (mySubAction);
        }
    } else {
        foreach (OpenVpn *configObj, subMenuList) {
            // Make new Menu from Config
            trayTest = new QMenu(configObj->configName, this);

            // Set Child of Menu
            menuChildList.append(mySubAction = new QAction(tr ("Connect"), this));
            mySubAction->setObjectName(configObj->configName);
            connect(mySubAction, SIGNAL(triggered()), configObj, SLOT(openConnect()));
            trayTest->addAction (mySubAction);

            menuChildList.append(mySubAction = new QAction(tr ("Disconnect"), this));
            mySubAction->setObjectName(configObj->configName);
            mySubAction->setDisabled(true);
            connect(mySubAction, SIGNAL(triggered()), configObj, SLOT(disconnectVpn()));
            trayTest->addAction (mySubAction);

            trayTest->addSeparator();

            menuChildList.append(mySubAction = new QAction(tr ("Show log"), this));
            mySubAction->setObjectName(configObj->configName);
            connect(mySubAction, SIGNAL(triggered()), configObj, SLOT(openVpnLog()));
            trayTest->addAction (mySubAction);

            menuChildList.append(mySubAction = new QAction(tr ("Edit config"), this));
            mySubAction->setObjectName(configObj->configName);
            connect(mySubAction, SIGNAL(triggered()), configObj, SLOT(editVpnConfig()));
            trayTest->addAction (mySubAction);

            menuChildList.append(mySubAction = new QAction(tr ("Change password"), this));
            mySubAction->setObjectName(configObj->configName);
            connect(mySubAction, SIGNAL(triggered()), configObj, SLOT(openConnect()));
            trayTest->addAction (mySubAction);

            // Append to Main Menu
            menuList.append(trayIconMenu->addMenu(trayTest));
        }
    }

    trayIconMenu->addSeparator();
    trayIconMenu->addAction(preferencesAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    // OpenVpn Objekte aktualisieren mit dem Systray
    foreach (OpenVpn *configObj, subMenuList) {
        configObj->setTray(trayIcon);
        configObj->setMenuChildList(menuChildList);
        configObj->setMenuList(menuList);
    }

}
