#ifndef DELETECONFIG_H
#define DELETECONFIG_H
#include <QtGui>
#include <QShowEvent>

#include "openvpn.h"
#include "mainwindowcontroll.h"

namespace Ui {
    class DeleteConfig;
}

class DeleteConfig : public QDialog {
    Q_OBJECT
public:
    static DeleteConfig *getInstance ();
    void setOpenVpnObject (OpenVpn *obj);
    OpenVpn *getOpenVpnObject ();

protected:
    void changeEvent(QEvent *e);
    void showEvent (QShowEvent *e);

private:
    static DeleteConfig *mInst;
    DeleteConfig();
    Ui::DeleteConfig *m_ui;
    OpenVpn *obj;
    QString getCAFromConfig ();
    QString getCertFromConfig ();
    QString getKeyFromConfig ();
    QString explorerPath;

private slots:
    void on_cmdDelete_clicked();
    void on_cmdCancel_clicked();
    void openExplorer ();
};

#endif // DELETECONFIG_H
