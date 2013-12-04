#ifndef MESSAGE_H
#define MESSAGE_H

#include <QtCore>
#include <QtGui>

class Message
{
public:
    static bool confirm (const QString &message, const QString &title = QObject::tr("Please confirm"), bool onTop = false);
    static void error (const QString &message, const QString &title = QObject::tr("An error occurred"));
    static void warning (const QString &message, const QString &tile = QObject::tr("User information"));
    static void information (const QString &message, const QString &tile = QObject::tr("User information"));

private:
    // Dont make a instance
    Message();
};

#endif // MESSAGE_H
