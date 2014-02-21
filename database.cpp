#include "database.h"
#include "debug.h"

#include "appfunc.h"
#include "settings.h"

#include "message.h"

Database::Database()
    : databasePath (AppFunc::getAppSavePath())
{
    // Make QSqlDatabase object
    if (!QSqlDatabase::database(QLatin1String("myConnection")).isValid()) {
        this->db = QSqlDatabase::addDatabase("QSQLITE", QLatin1String("myConnection"));
    } else {
        this->db = QSqlDatabase::database(QLatin1String("myConnection"));
    }

    if (Settings::getInstance()->getIsPortableClient()) {
        this->databasePath = qApp->applicationDirPath();
    }

    QString databaseFullPath (this->databasePath + QLatin1String("/vpn.sqlite"));
    Debug::log(QLatin1String("Database path: ") + databaseFullPath);

    bool createTables(false);
    if (!QFile::exists(databaseFullPath)) {
        // No Database was found
        //Message::error(QLatin1String("No database found. Create new database."));
        //
        // Build new database
        createTables = true;
    }

    // Set database path
    this->db.setDatabaseName(databaseFullPath);
    // Init database
    this->db.setConnectOptions("QSQLITE_BUSY_TIMEOUT=50");
    // Open Databse
    if (!this->open()) {
        Debug::error(QLatin1String("Can't open database!"));
        //
        QCoreApplication::exit(1);
    }
    this->execute("pragma key = '" + Settings::getInstance()->getCryptKey() + "';");
    Debug::log(QLatin1String("Databse: is open"));
    // Check if it is necessary to create the tables
    if (createTables) {
        Debug::log(QLatin1String("Create tables"));
        QLatin1String ssql ("CREATE TABLE \"vpn\" (\"vpn-id\" INTEGER PRIMARY KEY  NOT NULL, \"vpn-name\" VARCHAR, \"vpn-config\" VARCHAR, \"vpn-autostart\" INTEGER, \"vpn-user\" VARCHAR, \"vpn-password\" VARCHAR, \"vpn-pkcs12\" VARCHAR, \"vpn-http-user\" VARCHAR, \"vpn-http-password\" VARCHAR)");
        if (!this->execute(ssql)) {
            Debug::error(QLatin1String("Can't create tables!"));
            //
            QCoreApplication::exit(1);
        }
    }
}

Database::~Database()
{
    this->close();
}

bool Database::open()
{
    if (!this->db.open()) {
        Debug::error(QLatin1String("Database: Can't open Database"));
        return false;
    }

    return true;
}

void Database::close()
{
    if (this->db.isOpen()) {
        this->db.close();
    }

    this->db.removeDatabase(QLatin1String("myConnection"));
}

bool Database::execute(const QString &sql)
{
    bool retVal (true);
    QSqlQuery *query = new QSqlQuery (QSqlDatabase::database(QLatin1String("myConnection")));
    query->exec(sql);
    if (query->lastError().type() != QSqlError::NoError) {
        Debug::error(QLatin1String("Database: SQL: ") + query->lastError().text());
        Debug::log(QLatin1String("SQL for last error: ") + sql);
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
    query->exec(sql);

    // Error occurred?
    if (query->lastError().type() != QSqlError::NoError) {
        Debug::error(QLatin1String("Database: SQL: ") + query->lastError().text());
        Debug::log(QLatin1String("SQL for last error: ") + sql);
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
