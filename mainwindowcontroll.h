#ifndef MAINWINDOWCONTROLL_H
#define MAINWINDOWCONTROLL_H

#include <QtCore>
#include <QtGui>


class Preferences;
class OpenVpn;

class MainWindowControll
{
public:
    static MainWindowControll *getInstance ();
    bool isMainWindowOpen ();
    void setMainWindow (Preferences *mainW);
    void showTrayMessage (QString message);
    void setTrayIcon (QIcon icon);
    void enableTreeButtons ();
    void refreshDialog ();
    QMenu *getSystray ();
    void setConnectionStatus ();
    void refreshConfigs ();
    void setConfigToTop (OpenVpn *obj);


private:
    MainWindowControll();
    Preferences *mainWin;
    static MainWindowControll* mInst;
};

#endif // MAINWINDOWCONTROLL_H
