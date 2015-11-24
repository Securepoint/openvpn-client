#ifndef EDITCONFIG_H
#define EDITCONFIG_H

#include <QtGui>
#include <QShowEvent>

namespace Ui {
    class QuickEditWidget;
}

#include <dialogs/framelessdialog.h>

class QuickEditWidget : public FramelessDialog {
    Q_OBJECT
public:    
    QuickEditWidget(const QString &path, QWidget * parent = 0);

protected:
    void changeEvent(QEvent *e);
    void showEvent (QShowEvent *e);

private:
    Ui::QuickEditWidget *m_ui;        
    QString path;
    QString getContent ();

private slots:
    void on_cmdClose_clicked();
    void on_cmdSave_clicked();
};

#endif // EDITCONFIG_H
