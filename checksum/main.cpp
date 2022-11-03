#include <QCoreApplication>
#include "srvcli.h"
#include <string>
#include <iostream>

#include <QtCore>

int main(int argc, char *argv[])
{

    QString user;
    QString userSid;
    QString path;
    QString portable("0");
    QString sum;
    int id (-1);
    //
    for (int x = 1; x < argc; x++) {
        if(!strcmp(argv[x], "-user" ) && x + 1 < argc) {
            user = QString(argv[x + 1]);
            x++;
        } else if(!strcmp(argv[x], "-sid" ) && x + 1 < argc) {
            userSid = QString(argv[x + 1]);
            x++;
        } else if(!strcmp(argv[x], "-path" ) && x + 1 < argc) {
            path = QString(argv[x + 1]);
            x++;
        } else if(!strcmp(argv[x], "-sum" ) && x + 1 < argc) {
            sum = QString(argv[x + 1]);
            x++;
        } else if(!strcmp(argv[x], "-portable" )) {
            portable = "1";
        } else if(!strcmp(argv[x], "-id" ) && x + 1 < argc) {
            id = QString(argv[x + 1]).toInt();
            x++;
        }
    }

    // Check if all is available
    if (user.isEmpty() || userSid.isEmpty() || path.isEmpty() || sum.isEmpty() || id == -1) {
        return 1;
    }

    QString commandArgs = QString ("%1;%2;%3;%4;%5;%6")
                            .arg(id)
                            .arg(user)
                            .arg(userSid)
                            .arg(path)
                            .arg(sum)
                            .arg(portable);

    QCoreApplication a(argc, argv);


    SrvCLI::instance()->send(QLatin1String("WRITE_CHECKSUM"), commandArgs);

    QTimer::singleShot(200, []() {
        qApp->exit(0);
    });

    return a.exec();
}
