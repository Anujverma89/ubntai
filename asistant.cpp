#include "asistant.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include "urls.h"
#include "loading.h"
Asistant::Asistant(Ui::MainWindow *ui) {
    this->ui = ui;
}

Asistant::~Asistant(){
    delete ui;
};

void Asistant::makeRequest(Ui::MainWindow *ui) {
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Asistant::handleResponse);
    QLabel *textdata =  new QLabel("");
    this->ui->queryBox->setText("");
    this->ui->scrollArea->setWidget(textdata);

    QString question = ui->questionBox->text();
    qDebug() << "User Question: " << question;

    QString apiKey = "AIzaSyBpvVYplRkFV3-XzQ0kfUnCYRnfyxIAVXM";
    QString geminiUrl = QString("https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash:generateContent?key=%1").arg(apiKey);

    QNetworkRequest request(geminiUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Correct JSON structure
    QJsonObject textPart;
    textPart["text"] = question;

    QJsonArray partsArray;
    partsArray.append(textPart);

    QJsonObject contentObject;
    contentObject["parts"] = partsArray;

    QJsonArray contentsArray;
    contentsArray.append(contentObject);

    QJsonObject json;
    json["contents"] = contentsArray;

    QJsonDocument jsonDoc(json);
    QByteArray jsonData = jsonDoc.toJson();

    manager->post(request, jsonData);
}

void Asistant::handleResponse(QNetworkReply *reply) {

    if (reply->error() == QNetworkReply::NoError) {
        // closing the window after query successfull
        Loading *loader = new Loading();
        loader->closeDialog();
        QByteArray responseData = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(responseData);

        if (!doc.isNull() && doc.isObject()) {
            QJsonObject rootObj = doc.object();
            QJsonArray candidates = rootObj["candidates"].toArray();

            if (!candidates.isEmpty()) {
                QJsonObject firstCandidate = candidates[0].toObject();
                QJsonObject content = firstCandidate["content"].toObject();
                QJsonArray parts = content["parts"].toArray();

                if (!parts.isEmpty()) {
                    QString text = parts[0].toObject()["text"].toString();
                    QLabel *textdata =  new QLabel(text);
                    QString question = this->ui->questionBox->text();
                    QString userId = this->ui->userId->text();
                    this->ui->scrollArea->setWidget(textdata);
                    saveQuery(question, userId, responseData);
                    qDebug() << text;
                }
            }
        }

    } else {
        qDebug() << "Error: " << reply->errorString();
        qDebug() << "Response: " << reply->readAll();
    }
    reply->deleteLater();
}


void Asistant::handleSaveQuery(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError){
        qDebug()<<"Query response saved";
    }else{
        qDebug()<<"Error"<<reply->errorString();
        qDebug() << "Response: " << reply->readAll();
    }
    reply->deleteLater();
};

void Asistant::saveQuery(QString question, QString userid, QString answer){
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    connect(manager, &QNetworkAccessManager::finished, this, &Asistant::handleSaveQuery);
    QString saveQueryUrl = urls::saveQuery;
    QNetworkRequest request(saveQueryUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrlQuery queryData;
    queryData.addQueryItem("user_id",userid);
    queryData.addQueryItem("question",question);
    queryData.addQueryItem("answer",answer);
    manager->post(request,queryData.query(QUrl::FullyEncoded).toUtf8());
};


// we will have a model a light weight model that will be trained using configuratin of our local machine
// And then you can ask any question related to your system everything will be answered by that system

