#include "usercontroll.h"

#include <QtGui>

UserControll *UserControll::mInst = NULL;

UserControll *UserControll::getInstance() {
    if (!mInst)
        mInst = new UserControll ();
    return mInst;
}

UserControll::UserControll()
{
    this->configDirectory = "";
    this->privateKey = "";
    this->fileName = QString ("/auth.dat");
    this->configWithCryptIsAvailable = false;
}

void UserControll::setConfigDirectory(QString dir) {
    this->configDirectory = dir;
}

bool UserControll::isConfigUserDataAvailable() {
    // Ist die crypt file vorhanden?
    if (this->getConfigDirectory() == ""){
        this->setconfigWithCryptIsAvailable(false);
        return false;
    }

    QFile _cFile (this->getConfigDirectory() + this->getCryptFileName());
    if (_cFile.exists()) {
        this->setconfigWithCryptIsAvailable(true);
        return true;
    } else {
        this->setconfigWithCryptIsAvailable(false);
        return false;
    }
}

void UserControll::setPrivateKey(QString key) {
    this->privateKey = key;
}

QString UserControll::getConfigDirectory() {
    return this->configDirectory;
}

QString UserControll::getPrivateKey() {
    return this->privateKey;
}

QString UserControll::getUserNamePlain() {
    if (!this->isConfigUserDataAvailable())
        return QString ("");

    // Datei ist da, einlesen und auswerten
    QFile rFile (this->getConfigDirectory() + this->getCryptFileName());
    if (!rFile.open(QIODevice::ReadOnly))
        return QString ("");
    // Datei ist offen. Nun nach 1:: am Anfang suchen
    QString cryptedUserName = "";
    QTextStream in (&rFile);
    while (!in.atEnd()) {
        QString line = in.readLine();
        bool found = false;
        if (line.left(3) == "1::"){
            cryptedUserName = line.right(line.size() - 3);
            found = true;
        }
        if (found)
            break;
    }
    // Username da?
    if (cryptedUserName == "")
        return QString ("");

    // Username ist da
    rFile.close();
    Crypt::getInstance()->setSecretKey(this->getPrivateKey());
    return Crypt::getInstance()->cryptToPlainText(cryptedUserName);
}

QString UserControll::getUserPasswordPlain() {
    if (!this->isConfigUserDataAvailable())
        return QString ("");

    // Datei ist da, einlesen und auswerten
    QFile rFile (this->getConfigDirectory() + this->getCryptFileName());
    if (!rFile.open(QIODevice::ReadOnly))
        return QString ("");
    // Datei ist offen. Nun nach 2:: am Anfang suchen
    QString cryptedUserPassword = "";
    QTextStream in (&rFile);
    while (!in.atEnd()) {
        QString line = in.readLine();
        bool found = false;
        if (line.left(3) == "2::"){
            cryptedUserPassword = line.right(line.size() - 3);
            found = true;
        }
        if (found)
            break;
    }
    // Username da?
    if (cryptedUserPassword == "")
        return QString ("");

    // Username ist da
    rFile.close();
    Crypt::getInstance()->setSecretKey(this->getPrivateKey());
    return Crypt::getInstance()->cryptToPlainText(cryptedUserPassword);
}

void UserControll::setUserNameCrypt(QString plainName) {
    if (plainName == "")
        return;

    // Datei ist da, Usernamen suchen und schreiben
    // Falls der Username noch nicht da ist, anfügen
    QFile rFile (this->getConfigDirectory() + this->getCryptFileName());
    if (!rFile.open(QIODevice::ReadWrite))
        return;

    QTextStream in (&rFile);
    QString pass = "";
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.left(3) == "2::")
            pass = line;
    }
    // Wieder an den Anfang der Datei gehen
    rFile.close();
    QFile wFile (this->getConfigDirectory() + this->getCryptFileName());
    if (!wFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return;

    QTextStream out (&wFile);
    Crypt::getInstance()->setSecretKey(this->getPrivateKey());
    out << QString ("1::") << Crypt::getInstance()->cryptPlainText(plainName) << "\n";
    if (pass != "") {
        out << pass;
    }    
    wFile.close();
}

void UserControll::setUserPasswordCrypt(QString plainPwd) {
    if (plainPwd == "")
        return;

    // Datei ist da, Usernamen suchen und schreiben
    // Falls der Username noch nicht da ist, anfügen
    QFile rFile (this->getConfigDirectory() + this->getCryptFileName());
    if (!rFile.open(QIODevice::ReadWrite))
        return;

    QTextStream in (&rFile);
    QString user = "";
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.left(3) == "1::")
            user = line;
    }
    // Wieder an den Anfang der Datei gehen
    rFile.close();
    QFile wFile (this->getConfigDirectory() + this->getCryptFileName());
    if (!wFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return;

    QTextStream out (&wFile);
    Crypt::getInstance()->setSecretKey(this->getPrivateKey());
    out << QString ("2::") << Crypt::getInstance()->cryptPlainText(plainPwd) << "\n";
    if (user != "") {
        out << user;
    }
    wFile.close();
}

QString UserControll::getCryptFileName() {
    return this->fileName;
}

void UserControll::setconfigWithCryptIsAvailable(bool flag) {
    this->configWithCryptIsAvailable = flag;
}

bool UserControll::isConfigWithCryptAvailable() {
    return this->configWithCryptIsAvailable;
}

QString UserControll::getPrivateKeyPub() {
    return this->privateKey;
}

bool UserControll::isUserNameConfigWithCryptAvailable() {
    if (!this->isConfigUserDataAvailable())
        return false;
    // Datei ist da, einlesen und auswerten
    QFile rFile (this->getConfigDirectory() + this->getCryptFileName());
    if (!rFile.open(QIODevice::ReadOnly))
        return false;
    // Datei ist offen. Nun nach 1:: am Anfang suchen
    QTextStream in (&rFile);
    bool found = false;
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.left(3) == "1::"){
            found = true;
        }
    }
    rFile.close();
    return found;
}

bool UserControll::isUserPasswordConfigWithCryptAvailable() {
    if (!this->isConfigUserDataAvailable())
        return false;
    // Datei ist da, einlesen und auswerten
    QFile rFile (this->getConfigDirectory() + this->getCryptFileName());
    if (!rFile.open(QIODevice::ReadOnly))
        return false;
    // Datei ist offen. Nun nach 1:: am Anfang suchen
    QTextStream in (&rFile);
    bool found = false;
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.left(3) == "2::"){
            found = true;
        }
    }
    rFile.close();
    return found;
}
