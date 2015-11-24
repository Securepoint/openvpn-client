#include "configvalues.h"

ConfigValues *ConfigValues::mInst = 0;

ConfigValues *ConfigValues::instance()
{
    if (!mInst) {
        mInst = new ConfigValues;
    }

    return mInst;
}

ConfigValues::ConfigValues()
{
}

QString ConfigValues::valueFromConfigKey(const QString &ovpnPath, const QString &key) const
{
    //
    // Return the value of the given config key
    // If the value isn't found it returns an empty string
    //

    if (!QFile::exists(ovpnPath)) {
        // File not available
        return QString();
    }

    // File is available open it
    QFile config(ovpnPath);
    //
    if (!config.open(QIODevice::ReadOnly)) {
        // Crap
        return QString();
    }

    // File is open read lines
    QTextStream in(&config);
    //
    while (!in.atEnd()) {
        // Read line and trimm unnecessary spaces
        QString line (in.readLine());

        line = line.trimmed();

        // Check if the line starts with key
        if (line.startsWith(key + QLatin1String(" "), Qt::CaseInsensitive)) {
            // Key value
            line = line.right(line.size() - key.size());
            // Remove " with nothing

			//line = line.left(line.lastIndexOf("\" ") + 1);

            line = line.replace("\"", "");
            // Trim the line
            line = line.trimmed();

			line = line.left(line.lastIndexOf(" "));

            // Close file
            config.close();

            //
            return line;
        }
    }

    // Nothing found
    config.close();

    //
   return QString();
}

bool ConfigValues::isGivenPathAbsolute(const QString &path) const
{
    //
    // Check if the given path is absolute
    //

    return (path.indexOf(":") > 0);
}

QString ConfigValues::fileNameOfAbsolutePath(const QString &path) const
{
    //
    // Returns the file name of the given path
    //

    QFileInfo fileInfo (path);

    //
    return fileInfo.fileName();
}

void ConfigValues::changeKeyValueInConfig(const QString&ovpnPath, const QString &key, const QStringList &newValues) const
{
    if (!QFile::exists(ovpnPath)) {
        // File not available
        return;
    }

    QStringList lines;

    bool bIgnore = false;

    {
        // File is available open it
        QFile config(ovpnPath);
        //
        if (!config.open(QIODevice::ReadOnly)) {
            // Crap
            return;
        }

        // File is open read lines
        QTextStream in(&config);
        //
        while (!in.atEnd()) {
            // Read line and trimm unnecessary spaces
            QString line (in.readLine().trimmed());
            // Search for key
            if (line.startsWith(key, Qt::CaseInsensitive) && !bIgnore) {
                // Write new line
                for(auto newValue : newValues)
                {
                    lines << QString("%1 %2")
                        .arg(key)
                        .arg(newValue);
                }
                bIgnore = true;
            } else {
                lines << line;
            }
        }

        // Nothing found
        config.close();
    }

    // Write new config
    QFile config(ovpnPath);
    //
    if (!config.open(QIODevice::WriteOnly)) {
        // Crap
        return;
    }
    //
    foreach(QString line, lines) {
        config.write(line.toUtf8() + "\n");
    }

    config.close();
}

QStringList ConfigValues::valueFromConfigKeyList(const QString &ovpnPath, const QString &key) const
{
    //
    // Return the value of the given config key
    // If the value isn't found it returns an empty string
    //

    QStringList list;

    if (!QFile::exists(ovpnPath)) {
        // File not available
        return list;
    }

    // File is available open it
    QFile config(ovpnPath);
    //
    if (!config.open(QIODevice::ReadOnly)) {
        // Crap
        return list;
    }

    // File is open read lines
    QTextStream in(&config);
    //
    while (!in.atEnd()) {
        // Read line and trimm unnecessary spaces
        QString line (in.readLine().trimmed());
        // Search for key
        if (line.startsWith(key, Qt::CaseInsensitive)) {
            // Key value
            line = line.right(line.size() - key.size());
            // Remove " with nothing

			//line = line.left(line.lastIndexOf("\" ") + 1);

            line = line.replace("\"", "");
            // Trim the line
            line = line.trimmed();

			line = line.left(line.lastIndexOf(" "));

            // Close file
            config.close();

            //
            list.append(line);
        }
    }

    // Nothing found
    config.close();

    //
   return list;
}

void ConfigValues::changeKeyValueInConfig(const QString &ovpnPath, const QString &key, const QString &newValue) const
{
    if (!QFile::exists(ovpnPath)) {
        // File not available
        return;
    }

    QStringList lines;

    {
        // File is available open it
        QFile config(ovpnPath);
        //
        if (!config.open(QIODevice::ReadOnly)) {
            // Crap
            return;
        }

        // File is open read lines
        QTextStream in(&config);
        //
        while (!in.atEnd()) {
            // Read line and trimm unnecessary spaces
            QString line (in.readLine().trimmed());
            // Search for key
            if (line.startsWith(key, Qt::CaseInsensitive)) {
                // Write new line
                lines << QString("%1 %2")
                         .arg(key)
                         .arg(newValue);
            } else {
                lines << line;
            }
        }

        // Nothing found
        config.close();
    }

    // Write new config
    QFile config(ovpnPath);
    //
    if (!config.open(QIODevice::WriteOnly)) {
        // Crap
        return;
    }
    //
    foreach(QString line, lines) {
        config.write(line.toUtf8() + "\n");
    }

    config.close();
}

QString ConfigValues::pathOfFile(const QString &path) const
{
    //
    // Returns the path of the given file with an ending separator
    //

    QFileInfo fileInfo (path);

    //
    return fileInfo.dir().path();
}
