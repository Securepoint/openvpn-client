#ifndef IMPORTWIDGET_H
#define IMPORTWIDGET_H

#include <QtWidgets/QWidget>

namespace Ui {
    class ImportWidget;
}

class ImportWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ImportWidget(QWidget *parent = 0);
    ~ImportWidget();

protected:
    void paintEvent(QPaintEvent *pe);

private slots:
    void on_cmdBack_clicked();

    void on_cmdSelectImportFile_clicked();

    void on_cmdImport_clicked();

    void cryptKey(QString key);

private:
    Ui::ImportWidget *ui;
    QString cryptPassword;
};

#endif // IMPORTWIDGET_H
