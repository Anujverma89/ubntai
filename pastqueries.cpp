#include "pastqueries.h"
#include <QVBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QNetworkAccessManager>
#include "urls.h"
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

qint64 Pastqueries::page = 0 ;
QLineEdit* Pastqueries::pageno = nullptr;

Pastqueries::Pastqueries(QWidget *parent, Ui::MainWindow *ui){
    this->ui=ui;

    setWindowTitle("Past Queries");
    QVBoxLayout *mainContaner = new QVBoxLayout(this);

    // this frame will hold query frame and the button to go to next query
    QFrame *mainFrame = new QFrame();
    QVBoxLayout *innerMainContiner = new QVBoxLayout(mainFrame);

    // this frame will containe query
    QFrame *QueryContainerframe = new QFrame();
    QVBoxLayout *frameContainerLayout = new QVBoxLayout(QueryContainerframe);
    QueryContainerframe->setStyleSheet("background-color:#333333;");
    this->queryContainer = frameContainerLayout;





    // this is the main container for navigation button and showing the count
    QFrame *navigatorFrame = new QFrame();
    QHBoxLayout *navigatorLayout = new QHBoxLayout(navigatorFrame);
    // navigatorFrame->setStyleSheet("background-color:red");
    navigatorLayout->setContentsMargins(0,0,0,0);


    // this is the sub container for showing the count added inside the navigator layout
    QFrame *countFrame = new QFrame();
    QHBoxLayout *countLayout = new QHBoxLayout(countFrame);
    // countFrame->setStyleSheet("background-color:green");
    countLayout->setContentsMargins(0,0,0,0);

    // this is the navigator button frame which hold button
    QFrame *navigatorButtomFrame = new QFrame();
    QHBoxLayout * nvaigatorButtonLayout = new QHBoxLayout(navigatorButtomFrame);
    // navigatorButtomFrame->setStyleSheet("background-color:green");

    //the count buttons
    pageno = new QLineEdit("0");
    pageno->setReadOnly(true);
    pageno->setFixedSize(60,40);
    pageno->setAlignment(Qt::AlignCenter);

    // navigator buttons to move left and right implimentting pagination
    QPushButton *leftpush = new QPushButton("<");
    QPushButton *rigthpush = new QPushButton(">");
    QSpacerItem *horizontalSpacer = new QSpacerItem(100, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    this->right = rigthpush;
    this->left = leftpush;
    leftpush->setFixedSize(60,40);
    rigthpush->setFixedSize(60,40);
    nvaigatorButtonLayout->addWidget(leftpush);
    nvaigatorButtonLayout->addWidget(rigthpush);
    connect(leftpush,&QPushButton::clicked,this,&Pastqueries::decreasePageQuery);
    connect(rigthpush,&QPushButton::clicked,this,&Pastqueries::increasePageQuery);


    // adding counts in the count layout
    countLayout->addWidget(pageno);

    // we are adding button counts and spacer in navigator layout
    navigatorLayout->addWidget(navigatorButtomFrame);
    navigatorLayout->addItem(horizontalSpacer);
    navigatorLayout->addWidget(countFrame);

    //adding two navigator container and querycontainer inside a main container
    innerMainContiner->addWidget(navigatorFrame);
    innerMainContiner->addWidget(QueryContainerframe);

    //adding main frame container to the hbox layout
    mainContaner->addWidget(mainFrame);
    resize(400,500);

    // calling getPastQueries() to fetch past request;
    getPastQueries();
}














void Pastqueries::getPastQueries(){
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    connect(manager, &QNetworkAccessManager::finished, this, &Pastqueries::handleGetPastQueryReply);
    QString user_id = this->ui->userId->text();
    QString getPastQueryUrl = urls::getPastQuery;
    QNetworkRequest request(getPastQueryUrl);

    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    QString pageStr = QString::number(Pastqueries::page+1);
    Pastqueries::page+=1;
    QUrlQuery postData;
    postData.addQueryItem("user_id",user_id);
    postData.addQueryItem("pageno",pageStr);

    manager->post(request,postData.query(QUrl::FullyEncoded).toUtf8());
}


void Pastqueries::increasePageQuery(){
    this->left->setDisabled(false);
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    connect(manager, &QNetworkAccessManager::finished, this, &Pastqueries::handleGetPastQueryReply);
    QString user_id = this->ui->userId->text();
    QString getPastQueryUrl = urls::getPastQuery;
    QNetworkRequest request(getPastQueryUrl);

    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    QString pageStr = QString::number(Pastqueries::page+1);
    Pastqueries::page+=1;
    QUrlQuery postData;
    postData.addQueryItem("user_id",user_id);
    postData.addQueryItem("pageno",pageStr);
    manager->post(request,postData.query(QUrl::FullyEncoded).toUtf8());
}


void Pastqueries::decreasePageQuery(){
    if(Pastqueries::page > 1){
        this->right->setDisabled(false);
        QNetworkAccessManager *manager = new QNetworkAccessManager();
        connect(manager, &QNetworkAccessManager::finished, this, &Pastqueries::handleGetPastQueryReply);
        QString user_id = this->ui->userId->text();
        QString getPastQueryUrl = urls::getPastQuery;
        QNetworkRequest request(getPastQueryUrl);

        request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
        QString pageStr = QString::number(Pastqueries::page-1);
        Pastqueries::page-=1;
        QUrlQuery postData;
        postData.addQueryItem("user_id",user_id);
        postData.addQueryItem("pageno",pageStr);
        manager->post(request,postData.query(QUrl::FullyEncoded).toUtf8());
    }else{
        this->left->setDisabled(true);
    }
}








void Pastqueries::handleGetPastQueryReply(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError){
        // increasing the page by one on each request
        while (QLayoutItem *item = queryContainer->takeAt(0)) {
            if (QWidget *widget = item->widget()) {
                widget->deleteLater();  // Delete widget safely
            }
            delete item;  // Delete the layout item
        }
        pageno->setText(QString::number(Pastqueries::page));
        // setting page no in UI
        QByteArray data = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isArray()) {
            QJsonArray array = doc.array();

            if(array.size() < 10){
                this->right->setDisabled(true);
            }

            for (const QJsonValue &value : array) {
                QJsonObject obj = value.toObject();
                QString quesitiontext =  obj["question"].toString();
                QFrame *queryFrame = new QFrame();
                QHBoxLayout *queryLayout = new QHBoxLayout(queryFrame);
                QLabel *question = new QLabel(quesitiontext);
                queryFrame->setContentsMargins(0,0,0,0);
                QPushButton *showQuery = new QPushButton("See More");
                showQuery->setStyleSheet("height:30px;width:100px");
                queryLayout->addWidget(question);
                queryLayout->addWidget(showQuery);
                queryFrame->setStyleSheet("background-color:#2d2d2d");
                this->queryContainer->addWidget(queryFrame);
            }
        } else if (doc.isObject()) {
            QJsonObject obj = doc.object();
            QString quesitiontext =  obj["question"].toString();
            QFrame *queryFrame = new QFrame();
            QHBoxLayout *queryLayout = new QHBoxLayout(queryFrame);
            QLabel *question = new QLabel(quesitiontext);
            QPushButton *showQuery = new QPushButton("See More");
            showQuery->setStyleSheet("height:30px;width:100px");
            queryLayout->addWidget(question);
            queryLayout->addWidget(showQuery);
            queryFrame->setStyleSheet("background-color:#333333");
            this->queryContainer->addWidget(queryFrame);
        }

    }else{
        qDebug()<<reply->errorString();
        qDebug()<<reply->readAll();
    }
};




void Pastqueries::closeEvent(QCloseEvent *event) {
    page = 0;  // Reset static variable when closing the window
    event->accept();  // Accept the close event
}





Pastqueries::~Pastqueries(){
    delete this->ui;
    page=1;
    pageno = nullptr;
    delete this->queryContainer;
};
