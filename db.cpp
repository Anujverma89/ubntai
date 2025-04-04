#include "db.h"
#include <QDebug>
Db::Db() {}


QSqlDatabase Db::db;
bool Db::conn = false;

void Db::initializeDatabase() {
    Db::db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/home/anuj/Desktop/mgp/Ubntai/ab.db");


    // Open the database connection
    Db::conn = db.open();

    if (!conn) {
        qDebug() << "Could not connect to database: " << db.lastError().text();
    } else {
        QSqlQuery query;
        QString createTablesQuery = R"(
        CREATE TABLE IF NOT EXISTS UserTable (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id TEXT NOT NULL UNIQUE,
            email TEXT NOT NULL UNIQUE,
            timezone TEXT NOT NULL,
            profile_url TEXT NOT NULL
        );)";


        QString launchtableQuery = R"(
            CREATE TABLE IF NOT EXISTS FirstLaunch (
            launchId INTEGER PRIMARY KEY AUTOINCREMENT,
            ifFirstLaunch INTEGER NOT NULL
        );)";

        query.exec(launchtableQuery);


        if (!query.exec(createTablesQuery)) {
            qDebug() << "Error creating tables:" << query.lastError().text();
        } else {
            qDebug() << "Database connected  & initiated successfully.";

            QSqlQuery findUserQuery;
            findUserQuery.prepare("SELECT * FROM UserTable");
            if(!findUserQuery.exec()){
                qDebug() << "Error in reading data from User Database" << query.lastError().text();
            }else{
                if (!findUserQuery.next()) {
                    // if(findUserQuery.value(0).toInt() == 0){
                        QSqlQuery SetFirstlaunchQuery;
                        SetFirstlaunchQuery.prepare("INSERT INTO FirstLaunch (ifFirstLaunch) VALUES (?)");
                        SetFirstlaunchQuery.addBindValue(1);
                        if(!SetFirstlaunchQuery.exec()){
                            qDebug() << "Error Setting First Launch:" << query.lastError().text();
                        }else{
                            qDebug() << "First Launch Set Successfully.";
                        }
                    // }
                }
            }
        }


    }

}
