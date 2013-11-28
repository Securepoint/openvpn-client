#include "message.h"


Message::Message()
{
}


bool Message::confirm(const QString &message, const QString &title, bool onTop)
{
    bool ret (false);

    QMessageBox box;
    // Init box
    box.setStandardButtons(QMessageBox::Ok | QMessageBox::No);
    box.setDefaultButton(QMessageBox::No);
    //
    if (onTop) {
        // MessageBox is on Top
        box.setWindowFlags(Qt::WindowStaysOnTopHint);
    }
    // Set data
    box.setWindowTitle(title);
    box.setText(message);
    //
    switch (box.exec()) {
        case QMessageBox::Ok:
            ret = true;
            break;
         default:
            // All other stuff is false
            break;
    }

    return ret;
}

void Message::error(const QString &message, const QString &title)
{
    QMessageBox::critical(0, title, message);
}

void Message::warning(const QString &message, const QString &tile)
{
    QMessageBox::warning(0, tile, message);
}

void Message::information(const QString &message, const QString &tile)
{
    QMessageBox::information(0, tile, message);
}
