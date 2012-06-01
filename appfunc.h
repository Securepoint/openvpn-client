#ifndef APPFUNC_H
#define APPFUNC_H

#include <QtCore>
#include <QtGui>

class AppFunc : public QObject
{
public:    
    static bool isAppPortable ();
    static QString getOS ();
    static QString getAppSavePath ();

private:
    AppFunc();    
};

#endif // APPFUNC_H
