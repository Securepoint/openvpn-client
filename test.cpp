#include "test.h"

int Test::stdClone = -1;

void Test::connectVPN() {
    QString program = "./app/bin/openvpn";
    QStringList arguments;
    this->start(program, arguments);
    this->setupChildProcess();
    connect( this, SIGNAL(error ( QProcess::ProcessError) ), this->parent(), SLOT(showProcessError (QProcess::ProcessError)));
    connect( this, SIGNAL(readyReadStandardOutput()), this, SLOT(readProcessData()));
}

void Test::errorMessage(QProcess::ProcessError error) {
    QString errMessage;

    switch (error) {
        case QProcess::FailedToStart:
            errMessage = QString ("The process failed to start. Either the invoked program is missing, or you may have insufficient permissions to invoke the program.");
            break;
        case QProcess::Crashed:
            errMessage = QString ("The process crashed some time after starting successfully.");
            break;
        case QProcess::Timedout:
            errMessage = QString ("The last waitFor...() function timed out. The state of QProcess is unchanged, and you can try calling waitFor...() again.");
            break;
        case QProcess::WriteError:
            errMessage = QString ("An error occurred when attempting to write to the process. For example, the process may not be running, or it may have closed its input channel.");
            break;
        case QProcess::ReadError:
            errMessage = QString ("An error occurred when attempting to read from the process. For example, the process may not be running.");
            break;
        case QProcess::UnknownError:
            errMessage = QString ("An unknown error occurred. This is the default return value of error().");
            break;
        default:
            errMessage = QString ("No valid error code!");
            break;
    }

    qDebug() << errMessage;
}

void Test::readProcessData() {
    QByteArray line;
    QString output;

    line = this->readAllStandardOutput();
    output = QString::fromLatin1(line);
    qDebug() << output;
}
