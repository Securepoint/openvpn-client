#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSystemTrayIcon>
#include <QtCore/QEvent>
#include <QtGui/QShowEvent>
#include <QtGui/QMovie>

namespace Ui {
class FrmMain;
}

class WidgetFactory;

class FrmMain : public QMainWindow
{
    Q_OBJECT

    enum UpDateState {Init, UpToDate, UpdateError, NewVersion};

private:
    bool isReconnect;
    int tapCount;
    bool installingTap;


public:
    static FrmMain *instance ();
    static bool instanceCheck();
    ~FrmMain();

    void taskBarMove();
    WidgetFactory *mainWidget ();
    //
    QString version;

    void checkForNewConfigAndRefreshUI ();

    bool startDaemon();
    bool initDaemon();
    void connectToService();

    void showTrayIcon();
    void setMenuToShowWindow ();

    void sendClose();

    void setIcon();
    void checkIfSendToTrayIsNeeded();

    void SetReconnect(bool bReconnect) {
        this->isReconnect = bReconnect;
    }

    void showDiffieHellmanWarning();

protected:
    void showEvent(QShowEvent *event);
    void paintEvent(QPaintEvent*);
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);
public slots:
    void setDisconnected (int id);
    void setError (int id, QString message);
    void userInputIsNeeded (int id, int type);
    void receivedIP (int id, QString ip);
    void receivedReconnect (int id);
    void receivedTapControl(int type);
    void receivedTapCount(int count);
    void receivedRemoveTap (QString state);
    void saveUserData (int id, int type, QString value, bool save);
    void receivedStatus (int id, bool isConnected, bool isConnecting, int lastAction, QString ip);
    void refreshUI();
    void dummyReceived(quint32);
    void closeApplication ();
private slots:
    void on_cmdCloseWindow_clicked();
    void on_cmdSettingMenu_clicked();

    void trayActivated(QSystemTrayIcon::ActivationReason reason);

    void updateAvailable(QString version);

    void on_cmdUpdateState_clicked();

    void showHideTrayMenu();

    void showWizard();

private:
    static FrmMain* mInst;
    explicit FrmMain();
    Ui::FrmMain *ui;

    QImage *qArrowBottom;
    QImage *qArrowTop;
    QImage *qArrowLeft;
    QImage *qArrowRight;

    QImage *qCurrentArrow;

    QSystemTrayIcon *trayIcon;
    WidgetFactory *widgetFactory;

    QTimer *updateUITimer;

    bool m_bTaskBarAutoHide;
    void checkTaskBarAutoHideProperty ();
    QString updateVersion;
};

#endif // FRMMAIN_H
