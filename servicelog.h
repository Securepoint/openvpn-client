#ifndef SERVICELOG_H
#define SERVICELOG_H

#include <QtGui>
#include <QShowEvent>

#include "preferences.h"

namespace Ui {
    class ServiceLog;
}

// OD: Umgebaut zum Singleton

class ServiceLog : public QDialog {
    Q_OBJECT
public:
    static ServiceLog *getInstance ();
    ~ServiceLog();
    void appendToLog (QString message);
    void clearLog ();

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);
    void showEvent (QShowEvent *e);

private:
    static ServiceLog* mInst;
    Ui::ServiceLog *m_ui;
    ServiceLog();
    QTimer *timer;
    QStringList srvLog;

private slots:
    void on_cmdClose_clicked();
    void on_cmdSave_clicked();
    void on_cmdStopLog_clicked();
    void refreshData ();
};

#endif // SERVICELOG_H
