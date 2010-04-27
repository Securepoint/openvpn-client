#ifndef CONFIGEXPORT_H
#define CONFIGEXPORT_H

#include <QtGui>
#include <QProcess>
#include <QMessageBox>
#include <QShowEvent>

namespace Ui {
    class ConfigExport;
}

class ConfigExport : public QDialog {
    Q_OBJECT
public:
    static ConfigExport *getInstance ();
    void setConfigPath (QString path);

protected:
    void changeEvent(QEvent *e);
    void showEvent (QShowEvent *e);

private:
    static ConfigExport *mInst;
    Ui::ConfigExport *m_ui;
    ConfigExport();
    QString configPath;

private slots:
    void on_cmdOpenDir_clicked();
    void on_cmdExport_clicked();
    void on_cmdCancel_clicked();
};

#endif // CONFIGEXPORT_H
