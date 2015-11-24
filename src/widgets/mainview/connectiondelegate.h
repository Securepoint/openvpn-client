#ifndef CONNECTIONDELEGATE_H
#define CONNECTIONDELEGATE_H

#include <ListViewSingleButtonDelegate.h>

class ConnectionDelegate : public ListViewSingleButtonDelegate
{
    Q_OBJECT

public:
    ConnectionDelegate(QObject *parent = 0);
    ~ConnectionDelegate();

     void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
     QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
private:


protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // CONNECTIONDELEGATE_H
