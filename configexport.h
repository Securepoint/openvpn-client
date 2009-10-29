#ifndef CONFIGEXPORT_H
#define CONFIGEXPORT_H

#include <QtGui>
#include <QProcess>
#include <QMessageBox>

namespace Ui {
    class ConfigExport;
}

class ConfigExport : public QDialog {
    Q_OBJECT
public:
    ConfigExport(QWidget *parent = 0);
    ~ConfigExport();
    QString configPath;

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ConfigExport *m_ui;


private slots:
    void on_cmdOpenDir_clicked();
    void on_cmdExport_clicked();
    void on_cmdCancel_clicked();
};

#endif // CONFIGEXPORT_H
