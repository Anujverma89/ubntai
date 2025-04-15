#ifndef USER_H

#define USER_H

#include<QObject>
#include<QNetworkAccessManager>
#include<QNetworkReply>
#include<ui_mainwindow.h>
#include <QDateTime>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

// this file is responsible for dealing with user creation , updating users and managing avatar

class User : public QObject
{
    Q_OBJECT;
public:
    User();
    ~User();
    void static createUser(QString email);
    void static updateAvatar();
    void static initializeSystem();
    // working with db
    void addUser(const QString &userId, const QString &email, const QString &timezone, const QString &profileUrl);
    void getUser(Ui::MainWindow *ui);
    int isFirstLaunch();
    void setFirstLaunch();

    QNetworkAccessManager *manager;


    static QString user_id;
    static QString timeZoneID;

};

#endif // USER_H
