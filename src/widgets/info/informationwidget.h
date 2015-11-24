#ifndef INFORMATIONWIDGET_H
#define INFORMATIONWIDGET_H

#include <QtWidgets/QWidget>

namespace Ui {
    class InformationWidget;
}

class InformationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InformationWidget(QWidget *parent = 0);
    ~InformationWidget();

protected:
    void paintEvent(QPaintEvent *pe);

private slots:
    void on_cmdBack_clicked();

private:
    Ui::InformationWidget *ui;
};

#endif // INFORMATIONWIDGET_H
