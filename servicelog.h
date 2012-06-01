#ifndef SERVICELOG_H
#define SERVICELOG_H

#include <QtGui>
#include <QShowEvent>

namespace Ui {
    class ServiceLog;
}

class ServiceLog : public QDialog {
    Q_OBJECT
public:
    ServiceLog(int id);
    ~ServiceLog();    

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);
    void showEvent (QShowEvent *e);

private:    
    Ui::ServiceLog *m_ui;    
    QTimer *timer;
    QStringList srvLog;
    int openvpnId;

private slots: 
    void on_cmdClose_clicked();
    void on_cmdSave_clicked();
    void on_cmdStopLog_clicked();
    void refreshData ();
};

#endif // SERVICELOG_H
