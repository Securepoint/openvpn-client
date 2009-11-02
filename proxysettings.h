#ifndef PROXYSETTINGS_H
#define PROXYSETTINGS_H

#include <QtGui>

namespace Ui {
    class ProxySettings;
}

class ProxySettings : public QDialog {
    Q_OBJECT
public:
    ProxySettings(QWidget *parent = 0);
    ~ProxySettings();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ProxySettings *m_ui;

private slots:
    void on_rbUseManual_toggled(bool checked);
    void on_cmdSave_clicked();
    void on_cmdClose_clicked();
};

#endif // PROXYSETTINGS_H
