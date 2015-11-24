#ifndef RIGHTALIGNPUSHBUTTONMENU_H
#define RIGHTALIGNPUSHBUTTONMENU_H

#include <QtWidgets/QMenu>
#include <QtWidgets/QWidget>
#include <QtCore/QEvent>
#include <QtGui/QShowEvent>

class RightAlignPushButtonMenu : public QMenu
{
    Q_OBJECT

public:
    RightAlignPushButtonMenu(QWidget *parent);
    ~RightAlignPushButtonMenu();

private:
    void showEvent(QShowEvent *event);
};

#endif // RIGHTALIGNPUSHBUTTONMENU_H
