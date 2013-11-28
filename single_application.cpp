#include <QTimer>
#include <QByteArray>

#include "single_application.h"
#include "preferences.h"
#include "message.h"
#include "settings.h"

SingleApplication::SingleApplication(int &argc, char *argv[], const QString uniqueKey) : QApplication(argc, argv)
{
    sharedMemory.setKey(uniqueKey);
    if (sharedMemory.attach())
        _isRunning = true;
    else
    {
        _isRunning = false;
        // attach data to shared memory.
        QByteArray byteArray("0"); // default value to note that no message is available.
        if (!sharedMemory.create(byteArray.size()))
        {
            qDebug("Unable to create single instance.");
            return;
        }

        sharedMemory.lock();
        char *to = (char*)sharedMemory.data();
        const char *from = byteArray.data();
        memcpy(to, from, qMin(sharedMemory.size(), byteArray.size()));
        sharedMemory.unlock();

        // start checking for messages of other instances.
        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(checkForMessage()));
        timer->start(1000);
    }
}

// public slots.
bool SingleApplication::winEventFilter(MSG* msg, long* result) {
   if(msg->message == WM_QUERYENDSESSION) {
       // When a connection is online and the user activated the check
       if (Preferences::instance()->isConnectionActive() && Settings::getInstance()->checkWindowsShutdown()) {
           Message::warning(QObject::tr("You are still connected to a vpn network!\nPlease disconnect bevor you close your windows session.\nBy forcing the quit from windows malfunctions can occurred!"));
           *result = 0;
           return true;
       }
   } else if (msg->message == WM_ENDSESSION) {
       if (msg->wParam != 0) {
           Preferences::instance()->closeAllOpenConnections();
       }
   }

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
    if (!_isRunning)
            return false;
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
    Preferences::instance()->openDialog();
}


