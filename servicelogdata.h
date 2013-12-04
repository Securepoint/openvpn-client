#ifndef SERVICELOGDATA_H
#define SERVICELOGDATA_H

#include <QtCore>

class ServiceLogData
{
public:
    static ServiceLogData *instance ();
    void clearAll ();
    void clearId (int id);
    void append (int id, const QString &message);
    QStringList logs (int id) const;

private:
    static ServiceLogData *mInst;
    ServiceLogData();

    QList <QPair<int, QStringList*> > log;
};

#endif // SERVICELOGDATA_H
