#include "servicelogdata.h"

#include <widgets/settings/client/settings.h>
#include <QMessageBox>

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
    if(Settings::instance()->VpnLog()) {
        printf("%s\n", message.toLatin1().data());
    }

    QString customMessage (message);
    // Check if message contains dh key too small
    // Then append user information to it
    if (customMessage.contains(QLatin1String("dh key too small"), Qt::CaseInsensitive)) {
        // Append information
        customMessage.append(QString("\n%1\n")
                       .arg(QObject::tr("This version needs at least a serverside 1024 bit dh key. Please use the 2.0.15 client and contact your server administrator.\nhttps://support.securepoint.de/viewtopic.php?t=6216")));

        // Notify the user with a message box
        QMessageBox *notifyUser = new QMessageBox;
        // Delete message box on close
        notifyUser->setAttribute(Qt::WA_DeleteOnClose, true);
        //
        notifyUser->setStandardButtons(QMessageBox::Ok);
        notifyUser->setIcon(QMessageBox::Critical);
        // Set the unser information
        notifyUser->setWindowTitle(QObject::tr("DH-Key is to small!"));
        notifyUser->setText(QObject::tr("This version needs at least a serverside 1024 bit dh key. Please use the 2.0.15 client and contact your server administrator.\nhttps://support.securepoint.de/viewtopic.php?t=6216"));
        //
        notifyUser->show();
    }

    bool foundId (false);
    typedef QPair<int, QStringList*> myPair;
    foreach (myPair pair, this->log) {
        if (pair.first == id) {
            QStringList *ret = pair.second;
            ret->append(customMessage);
            foundId = true;
        }
    }

    // Wenn der Eintrag noch nicht in der Liste ist anfügen
    if (!foundId) {
        this->log.append(qMakePair(id, new QStringList(customMessage)));
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
