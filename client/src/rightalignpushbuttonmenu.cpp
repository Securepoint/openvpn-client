#include "rightalignpushbuttonmenu.h"
#include <qevent.h>
#include <QtWidgets/QPushButton>

RightAlignPushButtonMenu::RightAlignPushButtonMenu(QWidget *parent)
    : QMenu(parent)
{

}

RightAlignPushButtonMenu::~RightAlignPushButtonMenu()
{

}

void RightAlignPushButtonMenu::showEvent(QShowEvent *event)
{
    //
    //
    //

    QPushButton *button = static_cast<QPushButton*>(this->parentWidget());
    //
    if (button) {
        // Determine new geomentry
        // Align the menu on the right side

        QPoint point (button->parentWidget()->mapToGlobal(button->geometry().topLeft()));
        this->setGeometry(point.x() - (this->width() - button->width()), point.y() - this->height(), this->width(), this->height());

    }

    //
    event->accept();
}
