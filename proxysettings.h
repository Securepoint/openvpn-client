#ifndef PROXYSETTINGS_H
#define PROXYSETTINGS_H

#include <QtGui>
#include <QShowEvent>

namespace Ui {
    class ProxySettings;
}

class ProxySettings : public QDialog {
    Q_OBJECT
public:
    static ProxySettings *getInstance ();
    ~ProxySettings();

protected:
    void changeEvent(QEvent *e);
    void showEvent (QShowEvent *e);

private:
    ProxySettings();
    Ui::ProxySettings *m_ui;
    static ProxySettings *mInst;

private slots:
    void on_rbUseManual_toggled(bool checked);
    void on_cmdSave_clicked();
    void on_cmdClose_clicked();
};

#endif // PROXYSETTINGS_H
