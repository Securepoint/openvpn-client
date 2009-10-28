#ifndef DELETECONFIG_H
#define DELETECONFIG_H
#include <QtGui>
#include "preferences.h"

namespace Ui {
    class DeleteConfig;
}

class DeleteConfig : public QDialog {
    Q_OBJECT
public:
    DeleteConfig(QWidget *parent = 0);
    ~DeleteConfig();
    void openDialog ();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DeleteConfig *m_ui;

private slots:
    void on_cmdDelete_clicked();
    void on_cmdCancel_clicked();
};

#endif // DELETECONFIG_H
