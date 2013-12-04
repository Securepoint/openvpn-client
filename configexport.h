#ifndef CONFIGEXPORT_H
#define CONFIGEXPORT_H

#include <QtCore>
#include <QtGui>

namespace Ui {
    class ConfigExport;
}

class ConfigExport : public QDialog {
    Q_OBJECT
public:
    ConfigExport(const QString &path);

protected:
    void changeEvent(QEvent *e);
    void showEvent (QShowEvent *e);

private:    
    Ui::ConfigExport *m_ui;    
    QString configPath;

private slots:
    void on_cmdOpenDir_clicked();
    void on_cmdExport_clicked();
    void on_cmdCancel_clicked();
};

#endif // CONFIGEXPORT_H
