#ifndef CONFIGVALUES_H
#define CONFIGVALUES_H

#include <QtCore>

class ConfigValues
{
public:
    static ConfigValues *instance ();

    QString valueFromConfigKey(const QString &ovpnPath, const QString &key) const;
    bool isGivenPathAbsolute (const QString &path) const;
    QString fileNameOfAbsolutePath (const QString &path) const;
    QString pathOfFile (const QString &path) const;

    void changeKeyValueInConfig(const QString &ovpnPath, const QString &key, const QString &newValue) const;

private:
    static ConfigValues *mInst;
    ConfigValues();

};

#endif // CONFIGVALUES_H
