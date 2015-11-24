#ifndef LISTVIEWBUTTONDELEGATE_H
#define LISTVIEWBUTTONDELEGATE_H

#include <QtWidgets>
#include <QtWidgets/QStyledItemDelegate>


class ListViewButtonDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    enum ButtonState {Normal, Hover, Pressed};
    explicit ListViewButtonDelegate(QObject *parent = 0);

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const = 0;
    virtual bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) = 0;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void clearLastState () = 0;

protected:
    bool isBetweenLimits (const QRect &limit, QPoint value);

    QPersistentModelIndex lastItemIndex;
    void checkMouseActionOnButton (QEvent *event, QAbstractItemModel *model, const QModelIndex &index, const QRect &limits, int state, int role);

    QPair <int, int> drawButton (const QModelIndex &index, QPainter *painter, const QRect &rect, const QIcon &icon, int role) const;

    QIcon iconForType (int type, const QStyleOptionViewItem &option) const;

    int textWidth (const QString &text, const QFont &font) const;
    QString cropStringToWidth (const QString &text, const QFont &font, int width) const;
    bool eventFilter(QObject *obj, QEvent *event);
signals:
    void updateIndex(const QModelIndex &);
    void setCursor (Qt::CursorShape type);


};

#endif // LISTVIEWBUTTONDELEGATE_H
