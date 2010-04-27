#ifndef APPFUNC_H
#define APPFUNC_H

#include <QObject>
#include <QtCore>
#include <QtGui>

class AppFunc : public QObject
{
public:
    static AppFunc *getInstance ();
    bool isAppPortable ();
    QString getOS ();
    QString getAppSavePath ();

private:
    AppFunc();
    static AppFunc *mInst;
};

#endif // APPFUNC_H
