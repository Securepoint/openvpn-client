#ifndef FRMSAVEDDATA_H
#define FRMSAVEDDATA_H

#include <QtCore>
#include <QtGui>

namespace Ui {
    class FrmSavedData;
}

class FrmSavedData : public QDialog
{
    Q_OBJECT

public:
    explicit FrmSavedData(QStringList configs=QStringList(), QWidget *parent = 0);
    ~FrmSavedData();

protected:
    void showEvent(QShowEvent *e);

private slots:
    void on_pushButton_clicked();

private:
    Ui::FrmSavedData *ui;

    QStringList data;
    void refreshTree ();
};

#endif // FRMSAVEDDATA_H
