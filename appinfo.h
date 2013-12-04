#ifndef APPINFO_H
#define APPINFO_H

#include <QtGui>

namespace Ui {
    class appInfo;
}


// Die Appinfo wird nun eine Singleton-Klasse
// Zudem ist der Name von appInfo auf AppInfo
// zwecks Nomenklatur geändert.
class AppInfo : public QDialog {
    Q_OBJECT
public:
    static AppInfo *getInstance ();
    ~AppInfo();

protected:
    void changeEvent(QEvent *e);
    void showEvent(QShowEvent *e);

private:
    static AppInfo* mInst;
    Ui::appInfo *m_ui;
    AppInfo();

private slots:
    void on_cmdClose_clicked();
};

#endif // APPINFO_H
