#ifndef TEST_H
#define TEST_H

#include <unistd.h>
#include <QProcess>
#include <QDebug>


class Test : public QProcess
{
    static int stdClone;
public:
    Test(QObject *parent = 0)
        : QProcess(parent)
    {
        if (stdClone == -1)
            stdClone = ::dup(fileno(stdin));
    }
    void connectVPN ();

protected:
    void setupChildProcess()
    {
        ::dup2(stdClone, fileno(stdin));
    }

 private slots:
    void errorMessage (QProcess::ProcessError error);
    void readProcessData ();
};



#endif // TEST_H
