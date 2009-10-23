#ifndef APPFUNC_H
#define APPFUNC_H

#include <QObject>
#include <QtGui>

class AppFunc : public QObject
{
public:
    AppFunc();
    bool isAppPortable ();
    QString getOS ();
};

#endif // APPFUNC_H
