#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QtWidgets/QListView>
#include <QtWidgets/QWidget>

#include "connectionmodel.h"
#include "connectiondelegate.h"

class MainListView : public QListView
{
    Q_OBJECT

public:
    MainListView(QWidget *parent = 0);

    ConnectionDelegate delegate;
    ConnectionModel model;

private slots:
    void customContextMenuRequested(const QPoint &pos);
    void setCursorSlot (Qt::CursorShape type);
    void openConfigSettings();
    void openQuickEdit();
    void ContextConnect();
    void ContextExport();
    void ContextRemove();
    void ContextLog();
    void connectionButtonClicked(const QModelIndex&);
    void connectionButtonDClick(const QModelIndex&);
    void createChecksumOfConfig ();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // MAINVIEW_H
