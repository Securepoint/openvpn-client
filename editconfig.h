#ifndef EDITCONFIG_H
#define EDITCONFIG_H

#include <QtGui>
#include <QShowEvent>

namespace Ui {
    class EditConfig;
}

class EditConfig : public QDialog {
    Q_OBJECT
public:
    static EditConfig *getInstance ();
    void setPath (QString path);

protected:
    void changeEvent(QEvent *e);
    void showEvent (QShowEvent *e);

private:
    Ui::EditConfig *m_ui;
    EditConfig();
    static EditConfig *mInst;
    QString path;
    QString getContent ();

private slots:
    void on_cmdClose_clicked();
    void on_cmdSave_clicked();
};

#endif // EDITCONFIG_H
