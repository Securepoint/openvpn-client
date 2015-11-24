#ifndef MESSAGE_H
#define MESSAGE_H

#include <QtCore>

class Message
{
public:
    static bool confirm (const QString &message, const QString &title = QObject::tr("Please confirm"), bool onTop = false);
    static void error (const QString &message, const QString &title = QObject::tr("An error occurred"), QWidget * parent = nullptr);
    static void warning (const QString &message, const QString &tile = QObject::tr("User information"), QWidget * parent = nullptr);
    static void information (const QString &message, const QString &tile = QObject::tr("User information"), QWidget * parent = nullptr);

private:
    // Dont make a instance
    Message();
};

#endif // MESSAGE_H
