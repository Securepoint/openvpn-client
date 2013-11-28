#ifndef FRMUPDATESETTINGS_H
#define FRMUPDATESETTINGS_H

#include <QDialog>

namespace Ui {
    class FrmUpdateSettings;
}

class FrmUpdateSettings : public QDialog
{
    Q_OBJECT

public:
    explicit FrmUpdateSettings(QWidget *parent = 0);
    ~FrmUpdateSettings();

protected:
    void showEvent(QShowEvent *e);

private slots:
    void on_cmdCancel_clicked();

    void on_cmdSave_clicked();

    void on_cmdCangeSource_clicked();

    void on_cbDirectConnection_toggled(bool checked);

    void on_cbUseProxy_toggled(bool checked);

private:
    Ui::FrmUpdateSettings *ui;
};

#endif // FRMUPDATESETTINGS_H
