#include "database.h"

#include "utils.h"
#include "crypt.h"

Database *Database::mInst = 0;

Database *Database::instance()
{
    if (!mInst) {
        mInst = new Database;
    }

    return mInst;
}

Database::Database()
    : databasePath (Utils::userApplicationDataDirectory())
{
    //
    // Create the database connection
    //

    if (!QSqlDatabase::database(QLatin1String("myConnection")).isValid()) {
        this->db = QSqlDatabase::addDatabase("QSQLITE", QLatin1String("myConnection"));
    } else {
        this->db = QSqlDatabase::database(QLatin1String("myConnection"));
    }

    // Check if the path already exists, otherwise create it
    {
        QDir databaseDir (this->databasePath);
        //
        if (!databaseDir.exists()) {
            // Create path
            databaseDir.mkpath(this->databasePath);
        }
    }

    QString databaseFullPath (this->databasePath + QLatin1String("/sslvpn.sqlite"));

    bool createTables(false);
    if (!QFile::exists(databaseFullPath)) {
        // No Database was found
        //
        // Build new database
        createTables = true;
    }

    // Set database path
    this->db.setDatabaseName(databaseFullPath);
    // Init database
    this->db.setConnectOptions("QSQLITE_BUSY_TIMEOUT=50");
    // Open database
    if (!this->open()) {
        qWarning() << "Can't open database";
        //
        QCoreApplication::exit(1);
    }

    // Check if it is necessary to create the tables
    if (createTables) {
        // Connections table
        QLatin1String sqlConnectionTable ("CREATE TABLE \"vpn\" (\"vpn-id\" INTEGER PRIMARY KEY  NOT NULL, \"vpn-name\" VARCHAR, \"vpn-config\" VARCHAR, \"vpn-autostart\" INTEGER, \"vpn-user\" VARCHAR, \"vpn-password\" VARCHAR, \"vpn-pkcs12\" VARCHAR, \"vpn-http-user\" VARCHAR, \"vpn-http-password\" VARCHAR, \"vpn-userconfig\" INTEGER, \"vpn-state\" INTEGER, \"vpn-last-used\" INTEGER, \"vpn-last-connected\" INTEGER, \"vpn-remove-on-start\" INTEGER, \"vpn-last-error\" TEXT)");
        if (!this->execute(sqlConnectionTable)) {
            //
            QCoreApplication::exit(1);
        }

        // Settings table
        QLatin1String sqlSettingsTable ("CREATE TABLE \"settings\" (\"settings-name\" VARCHAR, \"settings-value\" VARCHAR)");
        if (!this->execute(sqlSettingsTable)) {
            //
            QCoreApplication::exit(1);
        }

        // Insert the default settings
        QStringList settingsSql;
        settingsSql << QString("INSERT INTO settings VALUES ('%1', '%2');")
            .arg(Crypt::encodePlaintext(QLatin1String("useInteract")))
            .arg(Crypt::encodePlaintext(QLatin1String("1")));
        //
        settingsSql << QString("INSERT INTO settings VALUES ('%1', '%2');")
            .arg(Crypt::encodePlaintext(QLatin1String("startOnWindows")))
            .arg(Crypt::encodePlaintext(QLatin1String("0")));
        //
        settingsSql << QString("INSERT INTO settings VALUES ('%1', '%2');")
            .arg(Crypt::encodePlaintext(QLatin1String("blockShutdown")))
            .arg(Crypt::encodePlaintext(QLatin1String("1")));
        //
        settingsSql << QString("INSERT INTO settings VALUES ('%1', '%2');")
            .arg(Crypt::encodePlaintext(QLatin1String("showBallon")))
            .arg(Crypt::encodePlaintext(QLatin1String("1")));
        //
        settingsSql << QString("INSERT INTO settings VALUES ('%1', '%2');")
            .arg(Crypt::encodePlaintext(QLatin1String("autoRestart")))
            .arg(Crypt::encodePlaintext(QLatin1String("1")));
        //
        settingsSql << QString("INSERT INTO settings VALUES ('%1', '%2');")
            .arg(Crypt::encodePlaintext(QLatin1String("alwaysPopup")))
            .arg(Crypt::encodePlaintext(QLatin1String("0")));
        //
        settingsSql << QString("INSERT INTO settings VALUES ('%1', '%2');")
            .arg(Crypt::encodePlaintext(QLatin1String("showSplash")))
            .arg(Crypt::encodePlaintext(QLatin1String("1")));
        //
        settingsSql << QString("INSERT INTO settings VALUES ('%1', '%2');")
            .arg(Crypt::encodePlaintext(QLatin1String("enableUpdate")))
            .arg(Crypt::encodePlaintext(QLatin1String("1")));
        //
        settingsSql << QString("INSERT INTO settings VALUES ('%1', '%2');")
            .arg(Crypt::encodePlaintext(QLatin1String("updateUrl")))
            .arg(Crypt::encodePlaintext(QLatin1String("http://updatevpnc.securepoint.de/vpn-changelog.xml")));
        //
        settingsSql << QString("INSERT INTO settings VALUES ('%1', '%2');")
            .arg(Crypt::encodePlaintext(QLatin1String("useSecurepoint")))
            .arg(Crypt::encodePlaintext(QLatin1String("1")));

        //
        settingsSql << QString("INSERT INTO settings VALUES ('%1', '%2');")
            .arg(Crypt::encodePlaintext(QLatin1String("maschineConfigDirectory")))
            .arg(Crypt::encodePlaintext(qApp->applicationDirPath() + QLatin1String("/configs")));

        /*settingsSql << QString("INSERT INTO settings VALUES ('%1', '%2');")
            .arg(Crypt::encodePlaintext(QLatin1String("proxy-use")))
            .arg(Crypt::encodePlaintext(QLatin1String("CONFIG")));

        settingsSql << QString("INSERT INTO settings VALUES ('%1', '%2');")
            .arg(Crypt::encodePlaintext(QLatin1String("proxy-type")))
            .arg(Crypt::encodePlaintext(QLatin1String("HTTP")));

        settingsSql << QString("INSERT INTO settings VALUES ('%1', '%2');")
            .arg(Crypt::encodePlaintext(QLatin1String("proxy-ip")))
            .arg(Crypt::encodePlaintext(QLatin1String("")));

                settingsSql << QString("INSERT INTO settings VALUES ('%1', '%2');")
            .arg(Crypt::encodePlaintext(QLatin1String("proxy-port")))
            .arg(Crypt::encodePlaintext(QLatin1String("")));*/

        // Write to disk
        foreach (QString sql, settingsSql) {
            this->execute(sql);
        }
    }
}

Database::~Database()
{
    this->close();
}

bool Database::open()
{
    return this->db.open();
}

void Database::close()
{
    if (this->db.isOpen()) {
        this->db.close();
    }

    this->db.removeDatabase(QLatin1String("myConnection"));
    //
    this->db = QSqlDatabase();
}

bool Database::execute(const QString &sql)
{
    bool retVal (true);
    QSqlQuery *query = new QSqlQuery (QSqlDatabase::database(QLatin1String("myConnection")));
    query->exec(sql);
    if (query->lastError().type() != QSqlError::NoError) {
        // TODO Warning
        printf("DB Error%s\n", query->lastError().text().toLatin1().data());
    }

    query->clear();
    delete query;
    query = 0;

    return retVal;
}

QSqlQuery* Database::openQuery(const QString &sql)
{
    //
    // Open a new query and retur the recordset
    //

    // Build new query
    // The ownership changes to the caller
    QSqlQuery *query = new QSqlQuery (QSqlDatabase::database(QLatin1String("myConnection")));
    // Exec query

    query->prepare(sql);
    query->exec();

    // Error occurred?
    if (query->lastError().type() != QSqlError::NoError) {
        // TODO Warning
    }

    return query;
}

QString Database::makeCleanValue(const QString &value)
{
    //
    // Removes the most sql injections commands
    //

    return value;
}

QString Database::join(const QSqlRecord &record, const QString &separator, bool setNewline)
{
    //
    // Joins the record with the given separator
    //

    QStringList recordData;    for (int fieldsCount = 0; fieldsCount < record.count(); fieldsCount++) {
        // Get valure from field
        QString fieldValue (record.value(fieldsCount).toByteArray().constData());
        // Mark invalid separator string
        fieldValue = fieldValue.replace(separator, QLatin1String("#INV-SEP#"));

        // Push value to value list
        recordData << fieldValue;
    }

    return recordData.join(separator) + (setNewline ? QLatin1String("\n") : QLatin1String(""));
}
