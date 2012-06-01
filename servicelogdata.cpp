#include "servicelogdata.h"

ServiceLogData *ServiceLogData::mInst = NULL;

ServiceLogData *ServiceLogData::instance()
{
    if (!mInst) {
        mInst = new ServiceLogData ();
    }

    return mInst;
}

ServiceLogData::ServiceLogData()
{
}

void ServiceLogData::clearAll()
{
    this->log.clear();
}

void ServiceLogData::clearId(int id)
{
    typedef QPair<int, QStringList*> myPair;
    foreach (myPair pair, this->log) {
        if (pair.first == id) {
            (static_cast<QStringList*>(pair.second))->clear();
            return;
        }
    }
}

void ServiceLogData::append(int id, const QString &message)
{    
    bool foundId (false);
    typedef QPair<int, QStringList*> myPair;
    foreach (myPair pair, this->log) {
        if (pair.first == id) {
            QStringList *ret = pair.second;
            ret->append(message);
            foundId = true;
        }
    }

    // Wenn der Eintrag noch nicht in der Liste ist anfügen
    if (!foundId) {
        this->log.append(qMakePair(id, new QStringList(message)));
    }
}

QStringList ServiceLogData::logs(int id) const
{        
    typedef QPair<int, QStringList*> myPair;
    foreach (myPair pair, this->log) {
        if (pair.first == id) {
            QStringList tmp ((*(pair.second)));
            return tmp;
        }
    }

    return QStringList();
}
