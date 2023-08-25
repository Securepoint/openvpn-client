#include "listviewbuttondelegate.h"

float windowsDpiScale();

ListViewButtonDelegate::ListViewButtonDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}


QSize ListViewButtonDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //
    // Returns the row size.
    //

    //
    Q_UNUSED (index)

    return QSize(option.rect.width(), 30 * windowsDpiScale());
}

bool ListViewButtonDelegate::isBetweenLimits(const QRect &limit, QPoint value)
{
    //
    // Returns true if the value is between the given limit
    //

    if(limit.contains(value)) {
        // Show the hand pointer if the user in within the button rect
        // Qt::PointingHandCursor	13
        emit setCursor(Qt::PointingHandCursor);

        //
        return true;
    } else {
        // User is not over the button
        // Qt::ArrowCursor	0
        emit setCursor(Qt::ArrowCursor);

        //
        return false;
    }
}

void ListViewButtonDelegate::checkMouseActionOnButton(QEvent *event, QAbstractItemModel *model, const QModelIndex &index, const QRect &limits, int state, int role)
{
    if (index != lastItemIndex) {
        // The index has been changed

        // Check if the cursor is over a mouse button
        QMouseEvent *mouse = static_cast<QMouseEvent*>(event);
        if (this->isBetweenLimits(limits, mouse->pos())) {
            // Hover
            // If the last item inex is valid, update the last index, because the state changed
            if (this->lastItemIndex.isValid()) {
                // Change from hover state to normal
                model->setData(this->lastItemIndex, (int) Normal, role);
                emit updateIndex(this->lastItemIndex);
            }
            // Is the new index a valid index?
            if (index.isValid()) {
                // Valid index
                // Update the model data with the new state->hover
                model->setData(index, state, role);
                // Update the view
                emit updateIndex(index);
                // Set the new current index
                this->lastItemIndex = index;
            } else {
                // Index is not valid, set default dummy
                this->lastItemIndex = QModelIndex ();
            }
        }
    } else {
        // Same index, but check the position of the cursor
        QMouseEvent *mouse = static_cast<QMouseEvent*>(event);
        if (!this->isBetweenLimits(limits, mouse->pos())) {
            // Update the model data with the new state->normal
            model->setData(index, (int) Normal, role);
        } else {
            // Update the model data with the new state->Hover
            model->setData(index, state, role);
        }
        // Update the view
        emit updateIndex(index);
    }
}

QPair<int, int> ListViewButtonDelegate::drawButton(const QModelIndex &index, QPainter *painter, const QRect &rect, const QIcon &icon, int role) const
{
     //
    // Draw a new button into the view with the given parameter
    //

    // Draw delete button
    QStyleOptionButton buttonStyle;

    //
    // Set the button state to enabled
    buttonStyle.state |= QStyle::State_Enabled;
    // Set postion and size to the item
    buttonStyle.rect = rect;
    // Button is a flat button
    buttonStyle.features = QStyleOptionButton::Flat;
    // No text, only an icon
    // Icon default is 16x16
    buttonStyle.iconSize = QSize(16* windowsDpiScale(), 16* windowsDpiScale());
    // Set the icon
    buttonStyle.icon = icon;
    //

    // Set the button states
    ButtonState state = (ButtonState) index.data(role).toInt();
    QPen pen;
    pen.setWidth(1);
    if (state == Hover) {
        // Button is hovered
        pen.setColor("#2c2c2c");
        painter->setBrush(QBrush("#F8F8FF"));

    } else if (state == Pressed) {
        // Button is pressed by the user
        pen.setColor("#cccccc");
    } else {
        // Draw the normal border


        pen.setColor("#000");
        painter->setBrush(QBrush("#EEEEEC"));
    }

    // Set the pen and draw the border of the button
    painter->setPen(pen);
    painter->drawRect(buttonStyle.rect);
    painter->setBrush(Qt::NoBrush);

    // Draw button
    QApplication::style()->drawControl(QStyle::CE_PushButton, &buttonStyle, painter);

    // Return the button limits for the mouse position check
    return qMakePair(rect.left(), rect.right());
}

QIcon ListViewButtonDelegate::iconForType(int type, const QStyleOptionViewItem &option) const
{
    Q_UNUSED(type)
    Q_UNUSED(option)

    //
    return QIcon();
}

int ListViewButtonDelegate::textWidth(const QString &text, const QFont &font) const
{
    //
    // Return the width of the given text
    //

    if (text.size() == 0) {
        //
        return 0;
    }

    QFontMetrics metrics (font);

    //
    return metrics.horizontalAdvance(text);

}

QString ListViewButtonDelegate::cropStringToWidth(const QString &text, const QFont &font, int width) const
{
    //
    // Chop the text to the given width
    //

    int textWidth (this->textWidth(text, font));
    // Check the text size
    if (textWidth <= width) {
        // All ok, return the text
        return text;
    }

    // Check zero size
    if (!width) {
        return QLatin1String("");
    }

    // Text is too long, chop it
    //
    // Figure out the width of the ... string with the given font
    int pointLenght(this->textWidth(QLatin1String("..."), font));

    // Subtract the width of ... from the width
    width = width - pointLenght;

    // Calculate the chars need to truncate
    // Figure out the space is needed for a single character
    double spaceForCharacter (qreal(textWidth) / qreal(text.size()));
    // How many characters fit in the available space
    double charactersForWidth (qreal(width) / spaceForCharacter);
    // We only need entire characters
    int charactersLeft (qRound(charactersForWidth));
    // Always one char less than calculated
    --charactersLeft;

    // Return choped text
    return text.left(charactersLeft) + QLatin1String("...");
}



bool ListViewButtonDelegate::eventFilter(QObject *obj, QEvent *event)
{
    return  QStyledItemDelegate::eventFilter(obj, event);
}
