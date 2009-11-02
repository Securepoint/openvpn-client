#ifndef EDITCONFIG_H
#define EDITCONFIG_H

#include <QtGui>

namespace Ui {
    class EditConfig;
}

class EditConfig : public QDialog {
    Q_OBJECT
public:
    EditConfig(QWidget *parent = 0);
    ~EditConfig();
    void setPath (QString path);
    void setContent (QString con);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::EditConfig *m_ui;

private slots:
    void on_cmdClose_clicked();
    void on_cmdSave_clicked();
};

#endif // EDITCONFIG_H
