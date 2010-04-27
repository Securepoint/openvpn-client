#include "settings.h"

Settings *Settings::mInst = NULL;

Settings *Settings::getInstance() {
    if (!mInst)
        mInst = new Settings ();
    return mInst;
}

Settings::Settings()
{
    this->autoReconnect = false;
    this->commandConfigPath = "";
    this->listenPort = 3655;
    this->servicePort = 3656;
    this->noBallonMessage = false;
    this->noUserAuth = false;
    this->runAsService = true;
    this->startCommandConfig = false;
    this->startConfigDir = false;
    this->startConfigDirPath = "";
    this->startWithPwd = false;
    this->startWithPwdPwd = "";
    this->startWithUser = false;
    this->startWithUserName = "";
    this->startNoPopUp = false;
    this->useCryptedData = false;
    this->forcePrivateKey = false;
    this->isLangGerman = true;
    this->useNoInteract = true;
    this->isPortableClient = false;
    this->manageClient = false;
}

// GETTER

bool Settings::getIsAutoReconnect() {
    return this->autoReconnect;
}

bool Settings::getIsShowNoBallonMessage() {
    return this->noBallonMessage;
}

bool Settings::getIsShowNoUserAuth() {
    return this->noUserAuth;
}

bool Settings::getIsRunAsSevice() {
    return this->runAsService;
}

bool Settings::getIsStartCommandConfig() {
    return this->startCommandConfig;
}

bool Settings::getIsStartConfigDir() {
    return this->startConfigDir;
}

bool Settings::getIsStartWithPwd() {
    return this->startWithPwd;
}

bool Settings::getIsStartWithUser() {
    return this->startWithUser;
}

bool Settings::getIsNoPopUp() {
    return this->startNoPopUp;
}

bool Settings::getUseCryptedData() {
    return this->useCryptedData;
}

bool Settings::getIsForcePrivateKey() {
    return this->forcePrivateKey;
}

bool Settings::getIsLanguageGerman() {
    return this->isLangGerman;
}

bool Settings::getUseNoInteract() {
    return this->useNoInteract;
}

bool Settings::getIsPortableClient() {
    return this->isPortableClient;
}

bool Settings::getIsManageClient() {
    return this->manageClient;
}

QString Settings::getCommandConfigPath() {
    return this->commandConfigPath;
}

QString Settings::getStartConfigDirPath() {
    return this->startConfigDirPath;
}

QString Settings::getStartWithPwdPwd() {
    return this->startWithPwdPwd;
}

QString Settings::getStartWithUser() {
    return this->startWithUserName;
}

quint16 Settings::getListenPort() {
    return this->listenPort;
}

quint16 Settings::getServerPort() {
    return this->servicePort;
}


// SETTER
void Settings::setIsAutoReconnect(bool flag) {
    this->autoReconnect = flag;
}

void Settings::setIsShowNoBallonMessage(bool flag) {
    this->noBallonMessage = flag;
}

void Settings::setIsShowNoUserAuth(bool flag) {
    this->noUserAuth = flag;
}

void Settings::setIsRunAsService(bool flag) {
    this->runAsService = flag;
}

void Settings::setIsStartCommandConfig(bool flag) {
    this->startCommandConfig = flag;
}

void Settings::setIsStartConfigDir(bool flag){
    this->startConfigDir = flag;
}

void Settings::setIsStartWithPwd(bool flag) {
    this->startWithPwd = flag;
}

void Settings::setIsStartWithUser(bool flag) {
    this->startWithUser = flag;
}

void Settings::setIsNoPopUp(bool flag) {
    this->startNoPopUp = flag;
}

void Settings::setUseCryptedData(bool flag) {
    this->useCryptedData = flag;
}

void Settings::setForcePrivateKey(bool flag) {
    this->forcePrivateKey = flag;
}

void Settings::setIsLanguageGerman(bool flag) {
    this->isLangGerman = flag;
}

void Settings::setUseNoInteract(bool flag) {
    this->useNoInteract = flag;
}

void Settings::setIsPortableClient(bool flag) {
    this->isPortableClient = flag;
}

void Settings::setIsManageClient(bool flag) {
    this->manageClient = flag;
}

void Settings::setCommandConfigPath(QString path) {
    this->commandConfigPath = path;
}

void Settings::setStartConfigDirPath(QString path) {
    this->startConfigDirPath = path;
}

void Settings::setStartWithPwdPwd(QString pwd) {
    this->startWithPwdPwd = pwd;
}

void Settings::setStartWithUserName(QString user) {
    this->startWithUserName = user;
}

void Settings::setServerPort(quint16 port) {
    this->servicePort = port;
}

void Settings::setListenPort(quint16 port) {
    this->listenPort = port;
}
