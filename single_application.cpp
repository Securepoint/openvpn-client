// "single_application.cpp"

#include <QTimer>
#include <QByteArray>
#include <QMessageBox>

#include "single_application.h"

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
       if (this->prefParent->isConnectionActive()) {
           QMessageBox msgBox;
           msgBox.setWindowTitle(tr("Securepoint SSL VPN"));
           msgBox.setText(tr("Exit Application"));
           msgBox.setWindowIcon(QIcon(":/images/logo.png"));
           msgBox.setInformativeText(tr("You are still connected to a vpn network!\nPlease disconnect bevor you close your windows session.\nBy forcing the quit from windows malfunctions can occurred!"));
           msgBox.setStandardButtons(QMessageBox::Ok);
           msgBox.setDefaultButton(QMessageBox::Ok);
           msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
           msgBox.exec();
           *result = 0;
           return true;
       }


   }
   return false;
};

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
    this->prefParent->openDialog(false, QString (""));
}


