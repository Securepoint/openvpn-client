#ifndef EXPORTWIDGET_H
#define EXPORTWIDGET_H

#include <QWidget>
#include "ui_exportwidget.h"

class ExportWidget : public QWidget
{
    Q_OBJECT

public:
    ExportWidget(QWidget *parent = 0);
    ~ExportWidget();

    void SetConfigPath(QString configPath);

protected:
    void paintEvent(QPaintEvent *pe);
    void showEvent(QShowEvent *event);

private slots:
    void on_cmdBack_clicked();
    void on_cmdExport_clicked();
    void on_cmdSelectImportFile_clicked();
private:
    Ui::ExportWidget ui;
    QString configPath;
};

#endif // EXPORTWIDGET_H
