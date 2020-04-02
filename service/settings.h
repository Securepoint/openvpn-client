#ifndef SETTINGS_H
#define SETTINGS_H


class Settings
{
public:
    static Settings* instance ();

    int tapDeviceTimeout () const;
    void setTapDeviceTimeout (int value);

    int tapInstallTimeout () const;
    void setTapInstallTimeout (int value);

    int tapRemoveTimeout () const;
    void setTapRemoveTimeout (int value);

    int tapCheckTimeout () const;
    void setTapCheckTimeout (int value);

private:
    Settings();
    static Settings *mInst;

    int tapDeviceTimeoutValue;
    int tapInstallTimeoutValue;
    int tapRemoveTimeoutValue;
    int tapCheckTimeoutValue;
};

#endif // SETTINGS_H
