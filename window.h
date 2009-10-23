#ifndef WINDOW_H
#define WINDOW_H

#include <QSystemTrayIcon>
#include <QDialog>
#include "Configs.h"
#include "openvpn.h"
#include "preferences.h"
#include "appfunc.h"

QT_BEGIN_NAMESPACE
class QAction;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMenu;
class QPushButton;
class QSpinBox;
class QTextEdit;
QT_END_NAMESPACE

//! [0]
class Window : public QDialog
{
    Q_OBJECT

public:
    Window();

    void setVisible(bool visible);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void manageConnections ();

private:
    // Methoden um feste Events erzeugen
    void createActions();
    // Methode um das Tray zu setzen
    void createTrayIcon();

    // Actions für Menu
    QAction *connectVpnAction;
    QAction *preferencesAction;
    QAction *quitAction;
    Preferences mydia;

    QAction *mySubAction;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QMenu *trayTest;

    // Configlisten
    Configs myConfigs;
    QList<OpenVpn*> subMenuList;
    // Menuliste
    QList<QAction*> menuList;
    QList<QAction*> menuChildList;
};
//! [0]

#endif
