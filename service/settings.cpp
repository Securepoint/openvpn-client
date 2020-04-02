#include "settings.h"

Settings *Settings::mInst = 0;

Settings *Settings::instance() {
    if (!mInst) {
        mInst = new Settings();
    }

    return mInst;
}

Settings::Settings()
    :
      tapDeviceTimeoutValue(40000),
      tapInstallTimeoutValue(180000),
      tapRemoveTimeoutValue(60000),
      tapCheckTimeoutValue(30000)
{

}


int Settings::tapDeviceTimeout() const
{
    return this->tapDeviceTimeoutValue;
}

void Settings::setTapDeviceTimeout(int value)
{
    this->tapDeviceTimeoutValue = value;
}

int Settings::tapInstallTimeout() const
{
    return this->tapInstallTimeoutValue;
}

void Settings::setTapInstallTimeout(int value)
{
    this->tapInstallTimeoutValue = value;
}

int Settings::tapRemoveTimeout() const
{
    return this->tapRemoveTimeoutValue;
}

void Settings::setTapRemoveTimeout(int value)
{
    this->tapRemoveTimeoutValue = value;
}

int Settings::tapCheckTimeout() const
{
    return this->tapCheckTimeoutValue;
}

void Settings::setTapCheckTimeout(int value)
{
    this->tapCheckTimeoutValue = value;
}
