#ifndef DB_H
#define DB_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
// Deals with database related tasks
class Db
{
public:
    Db();
    void initializeDatabase();

    static QSqlDatabase db;
    static bool conn;
};

#endif // DB_H
