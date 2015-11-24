#include "connectiondelegate.h"

#include <QtGui/QPainter>

float windowsDpiScale();

ConnectionDelegate::ConnectionDelegate(QObject *parent)
    : ListViewSingleButtonDelegate(parent)
{
    this->installEventFilter(this);
}

ConnectionDelegate::~ConnectionDelegate()
{

}

bool ConnectionDelegate::eventFilter(QObject *obj, QEvent *event)
{
    return ListViewSingleButtonDelegate::eventFilter(obj, event);
}

void ConnectionDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.isValid()) {
        ListViewSingleButtonDelegate::paint(painter, option, index);
    }

    QString name (index.model()->data(index, Qt::DisplayRole).toString());

    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor("#ffffff"));

    // Nun den Rahmen zeichnen
    painter->drawRect(option.rect);

    painter->save();

    painter->setPen(Qt::black);
    QStyleOptionViewItem nOption (option);
    nOption.rect.adjust(60 * windowsDpiScale(), 10 * windowsDpiScale(), -60 * windowsDpiScale(), 0);
    QFont font("Verdana", 12, QFont::Normal);
    //font.setPixelSize(16);
    painter->setFont(font);
    painter->drawText(nOption.rect, name);

    painter->restore();

    ListViewSingleButtonDelegate::paint(painter, option, index);
}

QSize ConnectionDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.isValid()) {
        return ListViewSingleButtonDelegate::sizeHint(option, index);
    }

     QString textBottom (index.model()->data(index, Qt::UserRole + 101).toString());

      if (textBottom == "__divider__") {
        return QSize (option.rect.width(), 15 * windowsDpiScale());
    }

    return QSize(option.rect.width(), 55 * windowsDpiScale());
}
