#include "message.h"

#include <QtWidgets>

Message::Message()
{
}

extern bool g_bSilent;


bool Message::confirm(const QString &message, const QString &title, bool onTop)
{
    if(g_bSilent)
        return true;
    
    bool ret (false);

    QMessageBox box;

    box.setStyleSheet("QPushButton:hover {background-color: rgb(195, 195, 195);} QPushButton {;text-align:center;padding-right:8px; padding-left: 8px;padding-top: 8px;padding-bottom: 8px;}");

    // Init box
    box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
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
        case QMessageBox::Yes:
            ret = true;
            break;
         default:
            // All other stuff is false
            break;
    }

    return ret;
}

void Message::error(const QString &message, const QString &title, QWidget * parent)
{
    if(g_bSilent)
        return;

    QMessageBox box(parent);

    box.setStyleSheet("QPushButton:hover {background-color: rgb(195, 195, 195);} QPushButton {;text-align:center;padding-right:8px; padding-left: 8px;padding-top: 8px;padding-bottom: 8px;}");

    // Init box
    box.setStandardButtons(QMessageBox::Ok);
    box.setDefaultButton(QMessageBox::Ok);
    box.setIcon(QMessageBox::Critical);

    // Set data
    box.setWindowTitle(title);
    box.setText(message);
    //
    switch (box.exec()) {
    default:
        break;
    }

    //QMessageBox::critical(parent, title, message);
}

void Message::warning(const QString &message, const QString &title, QWidget * parent)
{
    if(g_bSilent)
        return;

    QMessageBox box(parent);

    box.setStyleSheet("QPushButton:hover {background-color: rgb(195, 195, 195);} QPushButton {;text-align:center;padding-right:8px; padding-left: 8px;padding-top: 8px;padding-bottom: 8px;}");

    // Init box
    box.setStandardButtons(QMessageBox::Ok);
    box.setDefaultButton(QMessageBox::Ok);
    box.setIcon(QMessageBox::Warning);

    // Set data
    box.setWindowTitle(title);
    box.setText(message);
    //
    switch (box.exec()) {
    default:
        break;
    }
}

void Message::information(const QString &message, const QString &title, QWidget * parent)
{
    if(g_bSilent)
        return;

    QMessageBox box(parent);

    box.setStyleSheet("QPushButton:hover {background-color: rgb(195, 195, 195);} QPushButton {;text-align:center;padding-right:8px; padding-left: 8px;padding-top: 8px;padding-bottom: 8px;}");

    // Init box
    box.setStandardButtons(QMessageBox::Ok);
    box.setDefaultButton(QMessageBox::Ok);
    box.setIcon(QMessageBox::Information);

    // Set data
    box.setWindowTitle(title);
    box.setText(message);
    //
    switch (box.exec()) {
    default:
        break;
    }
}
