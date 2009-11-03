#ifndef TAPINFO_H
#define TAPINFO_H

#include <QtGui>


namespace Ui {
    class TapInfo;
}

class TapInfo : public QDialog {
    Q_OBJECT
public:
    TapInfo(QWidget *parent = 0);
    ~TapInfo();
    void openDialog ();

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);

private:
    Ui::TapInfo *m_ui;
    QTimer *timer;
    int lastPc;
    int lastPcOut;
    bool timerNewStarted;

private slots:
    void on_cmdClose_clicked();
    void refreshTraffic ();
};

#endif // TAPINFO_H
