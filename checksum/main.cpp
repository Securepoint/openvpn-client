#include <cstring>
#include <QString>
#include <QSettings>

int main(int argc, char *argv[])
{
    // Command line arguments
    QString user;
    QString userSid;
    QString path;
    QString portable("0");
    QString sum;
    int id(-1);

    // Parse command line arguments
    for (int x = 1; x < argc; x++) {
        if (!strcmp(argv[x], "-user") && x + 1 < argc) {
            user = QString(argv[x + 1]);
            x++;
        } else if (!strcmp(argv[x], "-sid") && x + 1 < argc) {
            userSid = QString(argv[x + 1]);
            x++;
        } else if (!strcmp(argv[x], "-path") && x + 1 < argc) {
            path = QString(argv[x + 1]);
            x++;
        } else if (!strcmp(argv[x], "-sum") && x + 1 < argc) {
            sum = QString(argv[x + 1]);
            x++;
        } else if (!strcmp(argv[x], "-portable")) {
            portable = "1";
        } else if (!strcmp(argv[x], "-id") && x + 1 < argc) {
            id = QString(argv[x + 1]).toInt();
            x++;
        }
    }

    // Check if all required arguments are available
    if (user.isEmpty() || userSid.isEmpty() || path.isEmpty() || sum.isEmpty() || id == -1) {
        return 1;
    }

    // No checksum needs to be stored in portable mode
    if (portable == "1") {
        return 0;
    }

    // Open the SSLVPN system registry settings
    QSettings registry(
        QSettings::NativeFormat,
        QSettings::SystemScope,
        "Securepoint",
        "SSLVPN"
    );

    // Store the checksum directly in the registry
    registry.setValue(
        QString("Users/%1/Security/%2/Checksum")
            .arg(userSid)
            .arg(id),
        sum
    );

    // Return an error if the checksum could not be written
    if (registry.status() != QSettings::NoError) {
        return 2;
    }

    return 0;
}