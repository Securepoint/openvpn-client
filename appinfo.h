#ifndef APPINFO_H
#define APPINFO_H

#include <QtGui/QDialog>

namespace Ui {
    class appInfo;
}

class appInfo : public QDialog {
    Q_OBJECT
public:
    appInfo(QWidget *parent = 0);
    ~appInfo();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::appInfo *m_ui;

private slots:
    void on_cmdClose_clicked();
};

#endif // APPINFO_H
