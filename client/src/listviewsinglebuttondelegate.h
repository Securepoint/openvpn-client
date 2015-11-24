#ifndef LISTVIEWSINGLEBUTTONDELEGATE_H
#define LISTVIEWSINGLEBUTTONDELEGATE_H

#include <QtWidgets>
#include "listviewbuttondelegate.h"

class ListViewSingleButtonDelegate : public ListViewButtonDelegate
{
    Q_OBJECT
public:
    explicit ListViewSingleButtonDelegate(QObject *parent = 0);

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

    void setButtonIcon(int type) {this->buttonIconType = type;}
    void clearLastState ();

protected:
    mutable QPair<int, int> buttonLimits;
    mutable QHash<int, QRect> rectForRow;

    int buttonIconType;
    bool eventFilter(QObject *obj, QEvent *event);

signals:
    void buttonClicked (const QModelIndex &);
    void buttonDClick (const QModelIndex &);

private:

};

#endif // LISTVIEWSINGLEBUTTONDELEGATE_H
