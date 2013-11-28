#ifndef VPNLOG_H
#define VPNLOG_H

#include <QtGui>

namespace Ui {
    class VpnLog;
}

class VpnLog : public QDialog {
    Q_OBJECT

public:
    VpnLog(QWidget *parent = 0);
    ~VpnLog();
    void setContent (const QString &con);
    QList<QString> *logList;
    void showDialog();

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);

private:
    Ui::VpnLog *m_ui;
    QTimer *timer;

private slots:
    void on_cmdStartStopLog_clicked();
    void on_cmdSave_clicked();
    void on_cmdClose_clicked();
    void refreshData ();
};

#endif // VPNLOG_H
