#ifndef CONFIGSETTINGSWIDGET_H
#define CONFIGSETTINGSWIDGET_H

#include <QWidget>
#include "ui_configsettingswidget.h"

class ConfigSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    ConfigSettingsWidget(QWidget *parent = 0);
    ~ConfigSettingsWidget();

    void SetId(int id);

    void Save();

protected:
    void paintEvent(QPaintEvent *pe);

private slots:
    void on_cmdBack_clicked();
    void on_cmdAdv_clicked();
    void on_cmdRemoveCreds_clicked();
    void on_cmdSetName_clicked();

private:
    Ui::ConfigSettingsWidget ui;

    void AddRemote();

    int id;
};

#endif // CONFIGSETTINGSWIDGET_H
