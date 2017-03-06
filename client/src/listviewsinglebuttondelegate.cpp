#include "listviewsinglebuttondelegate.h"

#include "widgets\mainview\connectiondata.h"

#include <frmmain.h>
#include <widgets/mainview/mainview.h>
#include <widgetfactory.h>

    QImage imgStart_;
    QImage imgOffline_;
    QImage imgError_;
    QImage imgOnline_;
    QImage imgStop_;
    QImage imgConnecting_;
    QImage imgIconUsers_;
    QImage imgIconAutostart_;
    QMovie * movieKeks;
    QSize _lastImgSize;

    QImage imgStart;
    QImage imgOffline;
    QImage imgError;
    QImage imgOnline;
    QImage imgStop;
    QImage imgConnecting;
    QImage imgIconUsers;
    QImage imgIconAutostart;


float windowsDpiScale();

ListViewSingleButtonDelegate::ListViewSingleButtonDelegate(QObject *parent)
    : ListViewButtonDelegate (parent),
      buttonLimits(qMakePair(0, 0)),
      buttonIconType(0)
{
    imgStart_.load(":/data/images/start.png");
    imgOffline_.load(":/data/images/offline.png");
    imgError_.load(":/data/images/error.png");
    imgOnline_.load(":/data/images/online.png");
    imgStop_.load(":/data/images/stop.png");
    imgConnecting_.load(":/data/images/loader.gif");
    imgIconUsers_.load(":/data/images/16_users.png");
    imgIconAutostart_.load(":/data/images/16_autostart.png");

    movieKeks = new QMovie(":/data/images/loader.gif");
    movieKeks->start();
}

#include <chrono>

void ListViewSingleButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
     //
    // This method paints a new row into the listview
    //

    painter->save();

    // Set normal background color
    painter->setBrush((QColor("#ffffff")));

    // Alternating rows
    if (index.row() % 2) {
        // Set brush to aliceblue
        painter->setBrush((QColor("#f5f5f5")));
    }

    if(index.model()->data(index, Qt::UserRole + 4).toInt() == ConnectionState::Error)
    {
        painter->setBrush(QColor("#FFCCCC"));
    }

    QString textBottom (index.model()->data(index, Qt::UserRole + 101).toString());
    QString textName (index.data(Qt::DisplayRole).toString());

    // Hindergrund zeichnen
    painter->setPen(Qt::NoPen);

    if (textBottom == "__divider__") {
        painter->setBrush(QBrush(QColor("#f2f2f2"))); //0053a1
        painter->drawRect(option.rect);
        painter->setPen(Qt::lightGray);
        painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
        if (index.row() == 0) {
            painter->drawLine(option.rect.topLeft(), option.rect.topRight());
        }

        painter->setPen(Qt::black);
        QStyleOptionViewItem nOptionD (option);
        QFont sdasda("Verdana", 10, QFont::Normal);
        //sdasda.setPixelSize(11);
        painter->setFont(sdasda);
        nOptionD.rect.adjust(3, 0, 0, 0);
        painter->drawText(nOptionD.rect, textName);
        rectForRow.insert(index.row(), QRect());
        rectForRow[index.row()] = option.rect;
         painter->restore();
        return;
    }

    painter->drawRect(option.rect);

    // Get text

    QString ip (index.data(Qt::UserRole + 1).toString());
    quint32 lastUsed(index.data(Qt::UserRole + 2).toUInt());
    quint32 lastConnected(index.data(Qt::UserRole + 3).toUInt());
    // Set and calc items width
    int buttonWidth (36* windowsDpiScale());
    int buttonHeight (36* windowsDpiScale());
    int marginRight (10* windowsDpiScale());
    int marginTop (5* windowsDpiScale());

    // Draw delete button
    QStyleOptionViewItem buttonOption (option);
    // -10 is the right margin, the margin is necessary to reset the state when mouse is leaving to the right side
    buttonOption.rect.setRight(buttonWidth + 2 * marginRight);
    buttonOption.rect.setHeight(buttonHeight);
    buttonOption.rect.adjust(10* windowsDpiScale(), option.rect.height() / 2 - buttonHeight / 2, -(marginRight), marginTop + 1);
    buttonOption.rect.setHeight(buttonHeight);
    // Draw delete button and return limits
    // This is to figure out which button is under the mouse position
    rectForRow.insert(index.row(), QRect());
    rectForRow[index.row()] = buttonOption.rect;

    auto drawButton = [](const QModelIndex &index, QPainter *painter, const QRect &rect, int role, ConnectionState conState)
    {
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
        //

        // Set the button states
        ButtonState state = (ButtonState) index.data(role).toInt();
        QPen pen;
        pen.setWidth(1);

        switch(conState)
        {
        case ConnectionState::Disconnected:
            {
                if(state == Hover) {
                    pen.setColor("#808080");
                    painter->setBrush(QBrush("#F0D151"));
                } else if(state == Pressed) {

                } else {
                    pen.setColor("#808080");
                    painter->setBrush(QBrush("#EEEEEC"));
                }
            }
            break;
        case ConnectionState::Connecting:
            {
                if(state == Hover) {
                    pen.setColor("#808080");
                    painter->setBrush(QBrush("#EEEEEC"));
                } else if(state == Pressed) {
                } else {
                    pen.setColor("#808080");
                    painter->setBrush(QBrush("#F1C40F"));
                }

            }
            break;
        case ConnectionState::Connected:
            {
                if(state == Hover) {
                    pen.setColor("#808080");
                    painter->setBrush(QBrush("#EEEEEC"));
                } else if(state == Pressed) {
                } else {
                    pen.setColor("#808080");
                    painter->setBrush(QBrush("#72C02C"));
                }
            }
            break;
        case ConnectionState::Error:
            {
                if(state == Hover) {
                    pen.setColor("#808080");
                    painter->setBrush(QBrush("#F0D151"));
                } else if(state == Pressed) {
                } else {
                    pen.setColor("#808080");
                    painter->setBrush(QBrush("#FF0000"));
                }
            }
            break;
        default:
            {
                pen.setColor("#808080");
                painter->setBrush(QBrush("#EEEEEC"));
            }
        }

    // Set the pen and draw the border of the button
    painter->setPen(pen);
    painter->drawRect(buttonStyle.rect);
    painter->setBrush(Qt::NoBrush);

    QRect buttonOption(rect);



    // Draw button
    QApplication::style()->drawControl(QStyle::CE_PushButton, &buttonStyle, painter);
    };

    ConnectionState conState = (ConnectionState)index.data(Qt::UserRole + 4).toInt();

    drawButton(index, painter, buttonOption.rect, Qt::UserRole + 100, conState);

   // this->buttonLimits = this->drawButton(index, painter, buttonOption.rect, this->iconForType(this->buttonIconType, option), Qt::UserRole + 100);


    // Draw text
    QStyleOptionViewItem textOption (option);
    textOption.rect.adjust(buttonWidth + marginRight + 10 + 4, 4, buttonWidth + 2 * marginRight, 0);
    QPen pen;
    pen.setColor(QColor("#2c2c2c"));
    painter->setPen(pen);

    QFont font1("Verdana", 9, QFont::Normal);
    //font1.setPixelSize(11);

    QFont font2("Verdana", 7, QFont::Normal);
    //font2.setPixelSize(9);

    painter->setFont(font1);
    painter->drawText(textOption.rect, textName);
    textOption.rect.adjust(0, 13* windowsDpiScale(), 0, 0);
    painter->setFont(font2);
    //painter->drawText(textOption.rect, "Connected: ");
    //textOption.rect.adjust(0, 9, 0, 0);
    //painter->drawText(textOption.rect, "Last used: ");
    //textOption.rect.adjust(0, 9, 0, 0);

    typedef std::chrono::duration<int, std::ratio_multiply<std::chrono::hours::period, std::ratio<24>>::type> days;
    std::chrono::seconds sec((QDateTime::currentDateTime().toTime_t() - lastConnected));

    auto d = std::chrono::duration_cast<days>(sec);
    auto h = std::chrono::duration_cast<std::chrono::hours>(sec - d);
    auto m = std::chrono::duration_cast<std::chrono::minutes>((sec - d - h));
    auto s = std::chrono::duration_cast<std::chrono::seconds>((sec - d - h - m));


    QString day("%1%2 D ");
    day = day.arg(d.count() < 10 ? "0" : "")
        .arg(d.count());

    QString hour("%1%2 H ");
    hour = hour.arg(h.count() < 10 ? "0" : "")
        .arg(h.count());

    QString minute("%1%2 M ");
    minute = minute.arg(m.count() < 10 ? "0" : "")
        .arg(m.count());

    switch(conState)
    {
    case ConnectionState::Connected:
        painter->drawText(textOption.rect, QObject::tr("Connected: ") + (day) + (hour) + (d.count() > 0 ? "" : minute));
        break;
    case ConnectionState::Disconnected:
        painter->drawText(textOption.rect, QObject::tr("Last used: ") + (lastUsed == 0 ? QObject::tr("Never") : QDateTime::fromTime_t(lastUsed).toString("dd.MM.yy hh:mm")));
        break;
    default:
        painter->drawText(textOption.rect, QObject::tr("Last used: ") + (lastUsed == 0 ? QObject::tr("Never") : QDateTime::fromTime_t(lastUsed).toString("dd.MM.yy hh:mm")));
        break;
    };

    if(buttonOption.rect.size() != _lastImgSize)
    {
        imgStart = imgStart_.scaled(buttonOption.rect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        imgOffline = imgOffline_.scaled(buttonOption.rect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        imgError = imgError_.scaled(buttonOption.rect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        imgOnline = imgOnline_.scaled(buttonOption.rect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        imgStop = imgStop_.scaled(buttonOption.rect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        imgConnecting = imgConnecting_.scaled(buttonOption.rect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        auto s = buttonOption.rect.size();

        // I dont know why this thing has to be adjusted, but well, it works.
        s.setWidth(s.width() - 1);
        s.setHeight(s.height() - 1);
        movieKeks->setScaledSize(s);

        QSize size(16, 16);



        imgIconUsers = imgIconUsers_.scaled(size * windowsDpiScale(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        imgIconAutostart = imgIconAutostart_.scaled(size * windowsDpiScale(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

        _lastImgSize = buttonOption.rect.size();
    }
    buttonOption.rect.adjust(((buttonWidth) - imgStart.width()) / 2 +1, ((buttonWidth) - imgStart.height()) / 2 + 1, 0, 0);

    ButtonState state = (ButtonState) index.data(Qt::UserRole + 100).toInt();

    switch(conState)
    {
    case ConnectionState::Disconnected:
        {
            if(state == Hover)
                painter->drawImage(buttonOption.rect.left(),  buttonOption.rect.top(), imgStart);
            else
                painter->drawImage(buttonOption.rect.left(),  buttonOption.rect.top(), imgOffline);
        }
        break;
    case ConnectionState::Connecting:
        {
            if(state == Hover)
                painter->drawImage(buttonOption.rect.left(),  buttonOption.rect.top(), imgStop);
            else
            {
                auto pixx = movieKeks->currentPixmap();
                //auto size = imgStart.size();
                ////size.setHeight(size.height() -10);
                //pixx = pixx.scaled(imgStart.size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                painter->drawImage(buttonOption.rect.left(),  buttonOption.rect.top(), pixx.toImage());
            }
        }
        break;
    case ConnectionState::Connected:
        {
            if(state == Hover)
                painter->drawImage(buttonOption.rect.left(),  buttonOption.rect.top(), imgStop);
            else
                painter->drawImage(buttonOption.rect.left(),  buttonOption.rect.top(), imgOnline);
        }
        break;
    case ConnectionState::Error:
        {
            if(state == Hover)
                painter->drawImage(buttonOption.rect.left(),  buttonOption.rect.top(), imgStart);
            else
                painter->drawImage(buttonOption.rect.left(),  buttonOption.rect.top(), imgError);
        }
        break;
    default:
        {
            if(state == Hover)
                painter->drawImage(buttonOption.rect.left(),  buttonOption.rect.top(), imgStart);
            else
                painter->drawImage(buttonOption.rect.left(),  buttonOption.rect.top(), imgOffline);
        }
    }


    static const int iconPadding = 5;

    textOption.rect.setTop(buttonOption.rect.bottom());
    textOption.rect.adjust(0, -imgIconAutostart.height() + 2, 0, 0);
    textOption.rect.setWidth(imgIconAutostart.width());

    if(index.data(Qt::UserRole + 5).toBool()) {
        painter->drawImage(textOption.rect.left(),  textOption.rect.top(), imgIconAutostart);
        textOption.rect.adjust(imgIconAutostart.width() + iconPadding, 0, 0, 0);
    }

    if(index.data(Qt::UserRole + 6).toBool()) {
        painter->drawImage(textOption.rect.left(),  textOption.rect.top(), imgIconUsers);
        textOption.rect.adjust(imgIconUsers.width() + iconPadding, 0, 0, 0);
    }

    painter->setPen(Qt::lightGray);
    painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());

    // Restore saved painter
    painter->restore();
}

void ListViewSingleButtonDelegate::clearLastState()
{
    //
    // Clears the last state of the index
    //

    if (this->lastItemIndex.isValid()) {
        // Reset edit and delete button
        // Ugly const cast, but it is necessary!
        QAbstractItemModel *model = const_cast<QAbstractItemModel*>(this->lastItemIndex.model());
        // Rest values to normal
        model->setData(this->lastItemIndex, (int) Normal, Qt::UserRole + 100);
        // Repaint the index
        emit updateIndex(this->lastItemIndex);
        //
        // Reset last index
        this->lastItemIndex = QModelIndex ();
    }
}

bool ListViewSingleButtonDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    //
    // The user clicked on the row
    // Figure out, if a button is hit
    //

    switch(event->type()) {
        case QEvent::MouseButtonRelease: {

            // Rest states
            this->checkMouseActionOnButton(event, model, index, rectForRow[index.row()], Hover, Qt::UserRole + 100);
            // Do the user clicked a button
            // Cast event to a mouse event
            QMouseEvent *mouse = static_cast<QMouseEvent*>(event);
            // Get the click position
            QPoint point (mouse->pos());
            // Is a button under the click position
            if (mouse->button() == Qt::MouseButton::LeftButton && this->isBetweenLimits(rectForRow[index.row()], point)) {
                emit buttonClicked(index);
            }
            break;
        }
        case QEvent::MouseMove: {
            this->checkMouseActionOnButton(event, model, index, rectForRow[index.row()], Hover, Qt::UserRole + 100);
            break;
        }
        case QEvent::MouseButtonPress: {
            this->checkMouseActionOnButton(event, model, index, rectForRow[index.row()], Pressed, Qt::UserRole + 100);
            break;
        }
        case QEvent::MouseButtonDblClick: {
            this->checkMouseActionOnButton(event, model, index, rectForRow[index.row()], Hover, Qt::UserRole + 100);
            QMouseEvent *mouse = static_cast<QMouseEvent*>(event);

            QPoint point (mouse->pos());
              if (!this->isBetweenLimits(rectForRow[index.row()], point)) {
                emit buttonDClick(index);
            }
            break;
        }
        default:
            return false;
            break;
    }

    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

bool ListViewSingleButtonDelegate::eventFilter(QObject *obj, QEvent *event)
{
    return ListViewButtonDelegate::eventFilter(obj, event);
}
