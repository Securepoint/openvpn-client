#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>

class Database
{
public:
    static Database *instance ();
    bool execute (const QString &sql);
    QSqlQuery *openQuery (const QString &sql);
    QString makeCleanValue (const QString &value);
    //
    void close ();
    //
    QString join(const QSqlRecord &record, const QString &separator = QLatin1String(";"), bool setNewline = true);

private:
    Database();
    ~Database();
    //
    static Database *mInst;
    //
    QSqlDatabase db;
    bool open ();
    QString databasePath;
};

#endif // DATABASE_H
