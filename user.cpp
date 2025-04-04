#include "user.h"
#include <QSysInfo>
#include <QDateTime>
#include <QTimeZone>
#include <QMessageBox>
#include <QInputDialog>
#include <QNetworkAccessManager>
#include <QUrlQuery>
#include "urls.h"
#include "db.h"

User::User(){
    manager = new QNetworkAccessManager(this);
};
User::~User(){};
QString User::user_id  = "";
QString User::timeZoneID  = "";

// logic to create user and save to database
void User::createUser(QString email){
    bool ok;
    QString email_id = email;
    QString user_id = User::user_id;
    QString timeZone = User::timeZoneID;
    QString profileUrl = "None";

    if(email == ""){
        QMessageBox::warning(nullptr,"Invalid Email","Please Enter a valid email");
    }else{

        //seding email to get otp making a request and getting it back
        QNetworkAccessManager manager;
        QString getotp = urls::getOtpUrl;
        QNetworkRequest request(getotp);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

        QUrlQuery postData;
        postData.addQueryItem("email", email);

        QNetworkReply *reply =  manager.post(request, postData.query(QUrl::FullyEncoded).toUtf8());

        // Wait for reply using QEventLoop
        QEventLoop loop;
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();  // Blocks until the request finishes
        QString otp;

        // Handle the response
        if (reply->error() == QNetworkReply::NoError) {

            otp = QInputDialog::getText(nullptr, "Enter OTP", "Enter your OTP : ",
                                                QLineEdit::Normal, "", &ok);
        } else {
            QMessageBox::warning(nullptr, "Error", "Failed to send OTP: " + reply->errorString());
        }

        reply->deleteLater();


        // after otp is entered
        if (ok && !otp.isEmpty()) {

            // logic to verify otp
            QNetworkAccessManager manager;
            QString verifyotpurl = urls::verifyOTPurl;
            QNetworkRequest request(verifyotpurl);
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

            QUrlQuery postData;
            postData.addQueryItem("email", email);
            postData.addQueryItem("otp", otp);


            QNetworkReply *reply =  manager.post(request, postData.query(QUrl::FullyEncoded).toUtf8());


            QEventLoop loop;
            QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
            loop.exec();  // Blocks until the request finishes


            /// after otp is verified logic to save data in the system
            if (reply->error() == QNetworkReply::NoError) {

                QString createuserurl = urls::createUserUrl;
                QNetworkRequest request(createuserurl);
                request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
                QUrlQuery userData;
                userData.addQueryItem("user_id",user_id);
                userData.addQueryItem("email",email);
                userData.addQueryItem("timezone",timeZone);
                userData.addQueryItem("profile_url",profileUrl);


                QNetworkReply *reply = manager.post(request,userData.query(QUrl::FullyEncoded).toUtf8());
                QEventLoop loop;
                QObject::connect(reply,&QNetworkReply::finished, &loop, &QEventLoop::quit);
                loop.exec();

                if(reply->error() == QNetworkReply::NoError){
                    // saving data to local sqlite
                    User Uone;
                    Uone.addUser(user_id, email, timeZone, profileUrl);
                    // displaying user
                    QMessageBox::information(nullptr, "Welcome !!!", "Your account has been successfylly created !!");
                }else{

                    QMessageBox::warning(nullptr, "Title", "Your warning message", QMessageBox::Ok);
                }
            } else {
                QMessageBox::warning(nullptr, "Error", "Failed to Validate OTP: " + reply->errorString());
            }

            reply->deleteLater();

        } else {
            QMessageBox::warning(nullptr, "OTP Required", "OTP entry was cancelled.");
        }
    }
};




void User::updateAvatar(){};


// this function generates a UserID for a user and save's it in a database along with other user information

void User::initializeSystem(){
    //fetching system information
    QString sysName = QSysInfo::prettyProductName();
    QString kernalType = QSysInfo::kernelType();
    QString kernalVersion = QSysInfo::kernelVersion();
    QString archType = QSysInfo::currentCpuArchitecture();
    QString hostname = QSysInfo::machineHostName();
    //getting current timezone
    QDateTime currentDateTime = QDateTime::currentDateTime();

    // Get current time (hour, minute, second)
    int hour = currentDateTime.time().hour();
    int minute = currentDateTime.time().minute();
    int second = currentDateTime.time().second();

    // Get current date (day, month, year)
    int day = currentDateTime.date().day();
    int month = currentDateTime.date().month();
    int year = currentDateTime.date().year();

    QTimeZone localTimeZone = QTimeZone::systemTimeZone();
    QString timeZoneId = localTimeZone.id();
    QStringList timeZoneFragments = timeZoneId.split("/");

    QString userId = QString("%7%1%2%3%4%5%6%8")
                        .arg(hour)
                        .arg(minute)
                        .arg(second)
                        .arg(day)
                        .arg(month)
                        .arg(year)
                        .arg(timeZoneFragments[0].toUpper())
                        .arg(hostname.toUpper());



    //userid = timeZoneContient+Hour+mintues+second+day+month+year+HostName,
    QNetworkAccessManager manager;
     QString addsystemurl = urls::addSystemUrl;
    QNetworkRequest request(addsystemurl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrlQuery postData;
    postData.addQueryItem("sys_name", sysName);
    postData.addQueryItem("kernel_type", kernalType);
    postData.addQueryItem("kernel_version", kernalVersion);
    postData.addQueryItem("arch_type", archType);
    postData.addQueryItem("hostname", hostname);
    postData.addQueryItem("timezone", timeZoneId);
    postData.addQueryItem("user_id", userId);

    QNetworkReply *reply =  manager.post(request, postData.query(QUrl::FullyEncoded).toUtf8());

    // Wait for reply using QEventLoop
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();  // Blocks until the request finishes

    // Handle the response
    if (reply->error() == QNetworkReply::NoError) {
        QMessageBox::information(nullptr, "System Initialized", "Initial configuration done !!");
    } else {
        QMessageBox::warning(nullptr, "Error", "Failed to create user: " + reply->errorString());
    }

    reply->deleteLater();

    //saving the time zone and user id
    User::user_id = userId;
    User::timeZoneID = timeZoneId;
};


// initializing database


// adding user to local database using sqlite
void User::addUser(const QString &userId, const QString &email, const QString &timezone, const QString &profileUrl) {
    if (!Db::conn) {
        qDebug() << "Error: Cannot open database -" << Db::db.lastError().text();
        return;
    }

    QSqlQuery query(Db::db);  // Use Db::db explicitly
    query.prepare("INSERT INTO UserTable (user_id, email, timezone, profile_url) VALUES (?, ?, ?, ?)");

    query.addBindValue(userId);
    query.addBindValue(email);
    query.addBindValue(timezone);
    query.addBindValue(profileUrl);

    if (!query.exec()) {
        qDebug() << "Error inserting user:" << query.lastError().text();
    } else {
        setFirstLaunch();
        qDebug() << "User added successfully!";
    }
}



// fetching user from database :
void User::getUser(Ui::MainWindow *ui) {
    // db = QSqlDatabase::addDatabase("QSQLITE");
    if (!Db::conn) {
        qDebug() << "Error: Cannot open database -" << Db::db.lastError().text();
        return;
    }

    QSqlQuery query(Db::db);
    if (!query.exec("SELECT * FROM UserTable")) {
        qDebug() << "Error fetching users:" << query.lastError().text();
        return;
    }

    while (query.next()) {
        ui->userId->setText(query.value("user_id").toString());
        ui->emailField->setText(query.value("email").toString());
        ui->timezonebutton->setText(query.value("timezone").toString());
        ui->emailField->setReadOnly(true);
    }
}

void User::setFirstLaunch(){
    int status ;
    if(!Db::conn){
        qDebug() << "Error: Cannot open database -" << Db::db.lastError().text();
        return;
    }
    QSqlQuery query(Db::db);  // Use Db::db explicitly
    query.prepare("UPDATE FirstLaunch set ifFirstLaunch = ? WHERE launchId = 1");
    query.addBindValue(0);

    if (!query.exec()) {
        qDebug() << "Error inserting user:" << query.lastError().text();
    } else {
        qDebug() << "User added successfully!";
    }
}




int User::isFirstLaunch(){
    int status = 2 ;
    if(!Db::conn){
        qDebug() << "Error: Cannot open database -" << Db::db.lastError().text();
        return 4;
    }
    QSqlQuery query(Db::db);
    if (!query.exec("SELECT ifFirstLaunch FROM FirstLaunch WHERE launchId = 1")) {
        qDebug() << "Error fetching users:" << query.lastError().text();
        return 5;
    }

    // Default value in case no row is found
    if (query.next()) {
        status = query.value(0).toInt();
    }
    qDebug()<<status;
    return status;

}



// when someone install's the software a UserID will be created and saved in a system
// now user will be prompted to ask and email compulsary
// now next time with email user can get his her userID | system id


// Initialization
// create a SQLite database
// create a user table
// check for user id
// if user exists do nothing
// if user doesnot exists create and save a user in current and remote database
// initialize the system configuration

// flow :
//1) Dialog opens
//2) enter's the email :
//3) checks for user :
//4) if user exits returns the user & email id
//5) after receving the email id and userid it is saved in the sqlite database and the welcome window terminates
//5) if user doesnot exists return false
//6) after receiving false the system call's the initialization the userid is achieved
//7) we will send a otp to the user
//8) otp is entered
//9) create user is called


