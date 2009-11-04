#ifndef RENAMECONFIG_H
#define RENAMECONFIG_H

#include <QtGui>
#include "preferences.h"

namespace Ui {
    class RenameConfig;
}

class RenameConfig : public QDialog {
    Q_OBJECT
public:
    RenameConfig(QWidget *parent = 0);
    ~RenameConfig();
    void setOldName (QString oldname);
    void setFullPath (QString path);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::RenameConfig *m_ui;
    QString fullPath;

private slots:
    void on_cmdRename_clicked();
    void on_cmdClose_clicked();
};

#endif // RENAMECONFIG_H
