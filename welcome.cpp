#include "welcome.h"
#include "urls.h"
#include "user.h"
#include <QVBoxLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QDebug>
#include <QLineEdit>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QEventLoop>
#include <qmessagebox.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>

Welcome::Welcome(QWidget *parent) {
    setWindowTitle("Welcome Geeks !!");

    QVBoxLayout *mainlayout = new QVBoxLayout(this);
    // layout->setAlignment(Qt::AlignVCenter);
    QFrame *frame = new QFrame();
    mainlayout->setContentsMargins(0,0,0,0);
    frame->setStyleSheet("background-color: #333333;");

    QVBoxLayout *frameLayout = new QVBoxLayout(frame);
    frameLayout->setContentsMargins(20, 10, 20, 40);;


    //creating and adding text label in a frame
    QLabel *introtext = new QLabel("Welcome to Ubntai\n Please enter your email to continue !!",this);
    introtext->setStyleSheet("font-size: 18px; font-weight: bold;");



    // creating a frame for a input and next button
    QFrame *form = new QFrame();
    QHBoxLayout *formFrame = new QHBoxLayout(form);
    formFrame->setContentsMargins(0,0,0,0);

    this->email = new QLineEdit();
    email->setStyleSheet("height:40px");
    email->setPlaceholderText("jhondoe@gmail.com");
    QPushButton *nextButton = new QPushButton("Next");
    nextButton->setStyleSheet("height:40px;");
    nextButton->setCursor(Qt::PointingHandCursor);
    //connecting button clicked
    connect(nextButton,&QPushButton::clicked,this,&Welcome::onNextButtonClicked);

    //adding email and next button in form frame to make it appear horizontally
    formFrame->addWidget(email);
    formFrame->addWidget(nextButton);

    //adding introtext and form in a frame in a vertical layout manner
    frameLayout->addWidget(introtext);
    frameLayout->addWidget(form);

    //adding frame to main layout which is vertical
    mainlayout->addWidget(frame);
    setLayout(mainlayout);
    resize(400,200);
    setFixedSize(400,200);
}

Welcome::~Welcome(){
    delete this->email;
}

void Welcome::onNextButtonClicked(){
    QString email = this->email->text();
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    connect(manager, &QNetworkAccessManager::finished, this, &Welcome::handleReply);
    QString url = urls::checkemailUrl;
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");

    QUrlQuery userData;
    userData.addQueryItem("email",email);

    manager->post(request,userData.query(QUrl::FullyEncoded).toUtf8());
}


void Welcome::handleReply(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError){
        QString status = reply->readAll();
        if(status == "False"){
            // initialize the user
            User::initializeSystem();
            //creating user and saving in database
            User::createUser(this->email->text());
            closeWindow();
        }else{
            //getting data from user and saving it here
            // User::addUser(user_id, email, timeZone, profileUrl);
            QJsonDocument jsonDoc = QJsonDocument::fromJson(status.toUtf8());

            // Check if it's a valid JSON
            if (!jsonDoc.isNull() && jsonDoc.isObject()) {
                QJsonObject jsonObj = jsonDoc.object();

                // Extract values
                QString userId = jsonObj["user_id"].toString();
                QString email = jsonObj["email"].toString();
                QString timezone = jsonObj["timezone"].toString();
                QString profileUrl = jsonObj["profile_url"].toString();
                qDebug()<<jsonObj;
                User uone;
                uone.addUser(userId, email, timezone, profileUrl);
                closeWindow();
            } else {
                qDebug() << "Invalid JSON!";
            }
        }
    }else{
        qDebug()<<reply->readAll();
    }
}


void Welcome::closeWindow(){
    this->close();
}


