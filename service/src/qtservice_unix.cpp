/****************************************************************************
**
** This file is part of a Qt Solutions component.
** 
** Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
** 
** Contact:  Qt Software Information (qt-info@nokia.com)
** 
** Commercial Usage  
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Solutions Commercial License Agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
** 
** GNU General Public License Usage 
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** 
** Please note Third Party Software included with Qt Solutions may impose
** additional restrictions and it is the user's responsibility to ensure
** that they have met the licensing requirements of the GPL, LGPL, or Qt
** Solutions Commercial license and the relevant license of the Third
** Party Software they are using.
** 
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** 
****************************************************************************/

#include "qtservice.h"
#include "qtservice_p.h"
#include "qtunixsocket.h"
#include "qtunixserversocket.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>
#include <QtCore/QFile>
#include <QtCore/QTimer>
#include <QtCore/QDir>
#include <pwd.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <signal.h>
#include <sys/stat.h>
#include <QtCore/QMap>
#include <QtCore/QSettings>
#include <QtCore/QProcess>

static QString encodeName(const QString &name, bool allowUpper = false)
{
    QString n = name.toLower();
    QString legal = QLatin1String("abcdefghijklmnopqrstuvwxyz1234567890");
    if (allowUpper)
        legal += QLatin1String("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    int pos = 0;
    while (pos < n.size()) {
	if (legal.indexOf(n[pos]) == -1)
	    n.remove(pos, 1);
	else
	    ++pos;
    }
    return n;
}

static QString login()
{
    QString l;
    uid_t uid = getuid();
    passwd *pw = getpwuid(uid);
    if (pw)
        l = QString(pw->pw_name);
    return l;
}

static QString socketPath(const QString &serviceName)
{
    QString sn = encodeName(serviceName);
    return QString(QLatin1String("/var/tmp/") + sn + QLatin1String(".") + login());
}

static bool sendCmd(const QString &serviceName, const QString &cmd)
{
    bool retValue = false;
    QtUnixSocket sock;
    if (sock.connectTo(socketPath(serviceName))) {
        sock.write(QString(cmd+"\r\n").toLatin1().constData());
	sock.flush();
        sock.waitForReadyRead(-1);
        QString reply = sock.readAll();
        if (reply == QLatin1String("true"))
            retValue = true;
	sock.close();
    }
    return retValue;
}

static QString absPath(const QString &path)
{
    QString ret;
    if (path[0] != QChar('/')) { // Not an absolute path
        int slashpos;
        if ((slashpos = path.lastIndexOf('/')) != -1) { // Relative path
            QDir dir = QDir::current();
            dir.cd(path.left(slashpos));
            ret = dir.absolutePath();
        } else { // Need to search $PATH
            char *envPath = ::getenv("PATH");
            if (envPath) {
                QStringList envPaths = QString::fromLocal8Bit(envPath).split(':');
                for (int i = 0; i < envPaths.size(); ++i) {
                    if (QFile::exists(envPaths.at(i) + QLatin1String("/") + QString(path))) {
                        QDir dir(envPaths.at(i));
                        ret = dir.absolutePath();
                        break;
                    }
                }
            }
        }
    } else {
        QFileInfo fi(path);
        ret = fi.absolutePath();
    }
    return ret;
}

QString QtServiceBasePrivate::filePath() const
{
    QString ret;
    if (args.isEmpty())
        return ret;
    QFileInfo fi(args[0]);
    QDir dir(absPath(args[0]));
    return dir.absoluteFilePath(fi.fileName());
}


QString QtServiceController::serviceDescription() const
{
    QSettings settings(QSettings::SystemScope, "QtSoftware");
    settings.beginGroup("services");
    settings.beginGroup(serviceName());

    QString desc = settings.value("description").toString();

    settings.endGroup();
    settings.endGroup();

    return desc;
}

QtServiceController::StartupType QtServiceController::startupType() const
{
    QSettings settings(QSettings::SystemScope, "QtSoftware");
    settings.beginGroup("services");
    settings.beginGroup(serviceName());

    StartupType startupType = (StartupType)settings.value("startupType").toInt();

    settings.endGroup();
    settings.endGroup();

    return startupType;
}

QString QtServiceController::serviceFilePath() const
{
    QSettings settings(QSettings::SystemScope, "QtSoftware");
    settings.beginGroup("services");
    settings.beginGroup(serviceName());

    QString path = settings.value("path").toString();

    settings.endGroup();
    settings.endGroup();

    return path;
}

bool QtServiceController::uninstall()
{
    QSettings settings(QSettings::SystemScope, "QtSoftware");
    settings.beginGroup("services");

    settings.remove(serviceName());

    settings.endGroup();
    settings.sync();

    QSettings::Status ret = settings.status();
    if (ret == QSettings::AccessError) {
        fprintf(stderr, "Cannot uninstall \"%s\". Cannot write to: %s. Check permissions.\n",
                serviceName().toLatin1().constData(),
                settings.fileName().toLatin1().constData());
    }
    return (ret == QSettings::NoError);
}


bool QtServiceController::start(const QStringList &arguments)
{
    if (!isInstalled())
        return false;
    if (isRunning())
        return false;
    return QProcess::startDetached(serviceFilePath(), arguments);
}

bool QtServiceController::stop()
{
    return sendCmd(serviceName(), QLatin1String("terminate"));
}

bool QtServiceController::pause()
{
    return sendCmd(serviceName(), QLatin1String("pause"));
}

bool QtServiceController::resume()
{
    return sendCmd(serviceName(), QLatin1String("resume"));
}

bool QtServiceController::sendCommand(int code)
{
    return sendCmd(serviceName(), QString(QLatin1String("num:") + QString::number(code)));
}

bool QtServiceController::isInstalled() const
{
    QSettings settings(QSettings::SystemScope, "QtSoftware");
    settings.beginGroup("services");

    QStringList list = settings.childGroups();

    settings.endGroup();

    QStringListIterator it(list);
    while (it.hasNext()) {
        if (it.next() == serviceName())
            return true;
    }

    return false;
}

bool QtServiceController::isRunning() const
{
    QtUnixSocket sock;
    if (sock.connectTo(socketPath(serviceName())))
	return true;
    return false;
}




///////////////////////////////////

class QtServiceSysPrivate : public QtUnixServerSocket
{
    Q_OBJECT
public:
    QtServiceSysPrivate();
    ~QtServiceSysPrivate();

    char *ident;

    QtServiceBase::ServiceFlags serviceFlags;

protected:
    void incomingConnection(int socketDescriptor);

private slots:
    void slotReady();
    void slotClosed();

private:
    QString getCommand(const QTcpSocket *socket);
    QMap<const QTcpSocket *, QString> cache;
};

QtServiceSysPrivate::QtServiceSysPrivate()
    : QtUnixServerSocket(), ident(0), serviceFlags(0)
{
}

QtServiceSysPrivate::~QtServiceSysPrivate()
{
    if (ident)
	delete[] ident;
}

void QtServiceSysPrivate::incomingConnection(int socketDescriptor)
{
    QTcpSocket *s = new QTcpSocket(this);
    s->setSocketDescriptor(socketDescriptor);
    connect(s, SIGNAL(readyRead()), this, SLOT(slotReady()));
    connect(s, SIGNAL(disconnected()), this, SLOT(slotClosed()));
}

void QtServiceSysPrivate::slotReady()
{
    QTcpSocket *s = (QTcpSocket *)sender();
    cache[s] += QString(s->readAll());
    QString cmd = getCommand(s);
    while (!cmd.isEmpty()) {
        bool retValue = false;
	if (cmd == QLatin1String("terminate")) {
            if (!(serviceFlags & QtServiceBase::CannotBeStopped)) {
                QtServiceBase::instance()->stop();
                QCoreApplication::instance()->quit();
                retValue = true;
            }
        } else if (cmd == QLatin1String("pause")) {
            if (serviceFlags & QtServiceBase::CanBeSuspended) {
                QtServiceBase::instance()->pause();
                retValue = true;
            }
        } else if (cmd == QLatin1String("resume")) {
            if (serviceFlags & QtServiceBase::CanBeSuspended) {
                QtServiceBase::instance()->resume();
                retValue = true;
            }
        } else if (cmd == QLatin1String("alive")) {
            retValue = true;
        } else if (cmd.length() > 4 && cmd.left(4) == QLatin1String("num:")) {
	    cmd = cmd.mid(4);
            QtServiceBase::instance()->processCommand(cmd.toInt());
            retValue = true;
	}
        QString retString;
        if (retValue)
            retString = QLatin1String("true");
        else
            retString = QLatin1String("false");
        s->write(retString.toLatin1().constData());
        s->flush();
	cmd = getCommand(s);
    }
}

void QtServiceSysPrivate::slotClosed()
{
    QTcpSocket *s = (QTcpSocket *)sender();
    s->deleteLater();
}

QString QtServiceSysPrivate::getCommand(const QTcpSocket *socket)
{
    int pos = cache[socket].indexOf("\r\n");
    if (pos >= 0) {
	QString ret = cache[socket].left(pos);
	cache[socket].remove(0, pos+2);
	return ret;
    }
    return "";
}

#include "release/qtservice_unix.moc"

bool QtServiceBasePrivate::sysInit()
{
    sysd = new QtServiceSysPrivate;
    sysd->serviceFlags = serviceFlags;
    // Restrict permissions on files that are created by the service
    ::umask(027);

    return true;
}

void QtServiceBasePrivate::sysSetPath()
{
    if (sysd)
        sysd->setPath(socketPath(controller.serviceName()));
}

void QtServiceBasePrivate::sysCleanup()
{
    if (sysd) {
        sysd->close();
        delete sysd;
        sysd = 0;
    }
}

bool QtServiceBasePrivate::start()
{
    if (sendCmd(controller.serviceName(), "alive")) {
        // Already running
        return false;
    }
    // Could just call controller.start() here, but that would fail if
    // we're not installed. We do not want to strictly require installation.
    ::setenv("QTSERVICE_RUN", "1", 1);  // Tell the detached process it's it
    return QProcess::startDetached(filePath(), args.mid(1), "/");
}

bool QtServiceBasePrivate::install(const QString &account, const QString &password)
{
    Q_UNUSED(account)
    Q_UNUSED(password)
    QSettings settings(QSettings::SystemScope, "QtSoftware");

    settings.beginGroup("services");
    settings.beginGroup(controller.serviceName());

    settings.setValue("path", filePath());
    settings.setValue("description", serviceDescription);
    settings.setValue("automaticStartup", startupType);

    settings.endGroup();
    settings.endGroup();
    settings.sync();

    QSettings::Status ret = settings.status();
    if (ret == QSettings::AccessError) {
        fprintf(stderr, "Cannot install \"%s\". Cannot write to: %s. Check permissions.\n",
                controller.serviceName().toLatin1().constData(),
                settings.fileName().toLatin1().constData());
    }
    return (ret == QSettings::NoError);
}

void QtServiceBase::logMessage(const QString &message, QtServiceBase::MessageType type,
			    int, uint, const QByteArray &)
{
    if (!d_ptr->sysd)
        return;
    int st;
    switch(type) {
        case QtServiceBase::Error:
	    st = LOG_ERR;
	    break;
        case QtServiceBase::Warning:
            st = LOG_WARNING;
	    break;
        default:
	    st = LOG_INFO;
    }
    if (!d_ptr->sysd->ident) {
	QString tmp = encodeName(serviceName(), TRUE);
	int len = tmp.toLocal8Bit().size();
	d_ptr->sysd->ident = new char[len+1];
	d_ptr->sysd->ident[len] = '\0';
	::memcpy(d_ptr->sysd->ident, tmp.toLocal8Bit().constData(), len);
    }
    openlog(d_ptr->sysd->ident, LOG_PID, LOG_DAEMON);
    foreach(QString line, message.split('\n'))
        syslog(st, "%s", line.toLocal8Bit().constData());
    closelog();
}

void QtServiceBase::setServiceFlags(QtServiceBase::ServiceFlags flags)
{
    if (d_ptr->serviceFlags == flags)
        return;
    d_ptr->serviceFlags = flags;
    if (d_ptr->sysd)
        d_ptr->sysd->serviceFlags = flags;
}

