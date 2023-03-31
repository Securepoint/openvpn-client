#include <QTimer>
#include <QByteArray>


#include "frmmain.h"

#include "single_application.h"
#include "message.h"
#include "settings/client/settings.h"

extern QString g_strClientName;

SingleApplication::SingleApplication(int &argc, char *argv[], const QString uniqueKey)
    : QApplication(argc, argv),
      _isRunning(false)
{
    sharedMemory.setKey(uniqueKey);
    if (sharedMemory.attach()) {
        _isRunning = true;

        sharedMemory.lock();
        char *to = (char*)sharedMemory.data();
        QString id(to);
        id = QString("%1").arg(id.toUInt() + 1);
        g_strClientName = "SPSSlVpnClient" + id;
        memcpy(to, id.toLatin1().data(), qMin(sharedMemory.size(), id.toLatin1().size()));
        sharedMemory.unlock();
    } else {
        _isRunning = false;
        // attach data to shared memory.
        QByteArray byteArray("1"); // default value to note that no message is available.
        if (!sharedMemory.create(25)) {
            qDebug("Unable to create single instance.");
            return;
        }

        g_strClientName = "SPSSlVpnClient1";
        sharedMemory.lock();
        char *to = (char*)sharedMemory.data();
        const char *from = byteArray.data();
        memcpy(to, from, qMin(sharedMemory.size(), byteArray.size()));
        sharedMemory.unlock();
    }
}

SingleApplication::SingleApplication(int &argc, char *argv[])
    : QApplication(argc, argv),
      _isRunning(false)
{

}

void SingleApplication::setSharedKey(const QString &uniqueKey)
{
    sharedMemory.setKey(uniqueKey);
    if (sharedMemory.attach()) {
        _isRunning = true;

        sharedMemory.lock();
        char *to = (char*)sharedMemory.data();
        QString id(to);
        id = QString("%1").arg(id.toUInt() + 1);
        g_strClientName = "SPSSlVpnClient" + id;
        memcpy(to, id.toLatin1().data(), qMin(sharedMemory.size(), id.toLatin1().size()));
        sharedMemory.unlock();
    } else {
        _isRunning = false;
        // attach data to shared memory.
        QByteArray byteArray("1"); // default value to note that no message is available.
        if (!sharedMemory.create(25)) {
            qDebug("Unable to create single instance.");
            return;
        }

        g_strClientName = "SPSSlVpnClient1";
        sharedMemory.lock();
        char *to = (char*)sharedMemory.data();
        const char *from = byteArray.data();
        memcpy(to, from, qMin(sharedMemory.size(), byteArray.size()));
        sharedMemory.unlock();
    }
}

// public slots.
bool SingleApplication::winEventFilter(MSG* msg, long* result) {
   if(msg->message == WM_QUERYENDSESSION) {
       qDebug() << "Query";
         // When a connection is online and the user activated the check

   }

   Q_UNUSED(msg)
   Q_UNUSED(result)

   return false;
}

void SingleApplication::checkForMessage()
{
    sharedMemory.lock();
    QByteArray byteArray = QByteArray((char*)sharedMemory.constData(), sharedMemory.size());
    sharedMemory.unlock();
    if (byteArray.left(1) == "0")
        return;
    byteArray.remove(0, 1);
    QString message = QString::fromUtf8(byteArray.constData());
    emit messageAvailable(message);

    // remove message from shared memory.
    byteArray = "0";
    sharedMemory.lock();
    char *to = (char*)sharedMemory.data();
    const char *from = byteArray.data();
    memcpy(to, from, qMin(sharedMemory.size(), byteArray.size()));
    sharedMemory.unlock();
}

// public functions.

bool SingleApplication::isRunning()
{
    return _isRunning;
}

bool SingleApplication::sendMessage(const QString &message)
{
    if (!_isRunning) {
            return false;
    }

    QByteArray byteArray("1");
    byteArray.append(message.toUtf8());
    byteArray.append('\0'); // < should be as char here, not a string!
    sharedMemory.lock();
    char *to = (char*)sharedMemory.data();
    const char *from = byteArray.data();
    memcpy(to, from, qMin(sharedMemory.size(), byteArray.size()));
    sharedMemory.unlock();

    return true;
}

void SingleApplication::receiveMessage(QString message) {
    Q_UNUSED (message)

    FrmMain::instance()->show();
}


