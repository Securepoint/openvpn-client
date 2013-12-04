#ifndef DATABASE_H
#define DATABASE_H

#include <QtCore>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

class Database
{
public:
    Database();
    ~Database();
    bool execute (const QString &sql);
    QSqlQuery *openQuery (const QString &sql);
    QString makeCleanValue (const QString &value);
    //
    void close ();
    //
    QString join(const QSqlRecord &record, const QString &separator = QLatin1String(";"), bool setNewline = true);

private:
    QSqlDatabase db;
    bool open ();    
    QString databasePath;    
};

#endif // DATABASE_H
