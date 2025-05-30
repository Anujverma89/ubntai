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
    auto *manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Asistant::handleResponse);

    QString question = ui->questionBox->text();
    qDebug() << "User Question: " << question;

    QUrl geminiUrl(urls::askQuestion);
    QNetworkRequest request(geminiUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Build the same JSON you showed
    QJsonObject textPart;
    textPart["question"] = question;
    QJsonArray partsArray { textPart };
    QJsonObject contentObject;
    contentObject["parts"] = partsArray;
    QJsonArray contentsArray { contentObject };
    QJsonObject root;
    root["contents"] = contentsArray;

    QJsonDocument doc(root);
    manager->post(request, doc.toJson());
}




void Asistant::handleResponse(QNetworkReply *reply) {

    if (reply->error() == QNetworkReply::NoError) {
        // Close the loading dialog
        Loading *loader = new Loading();
        loader->closeDialog();

        QByteArray responseData = reply->readAll();
        qDebug()<<responseData;
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

                    // Convert **bold** markdown to <b>bold</b> HTML
                    QString htmlText = text;
                    htmlText.replace("**", "<b>");
                    bool toggle = true;
                    int pos = 0;
                    while ((pos = htmlText.indexOf("<b>", pos)) != -1) {
                        if (toggle) {
                            pos += 3; // skip past <b>
                        } else {
                            htmlText.replace(pos, 3, "</b>");
                            pos += 4;
                        }
                        toggle = !toggle;
                    }

                    // Preserve line breaks by replacing '\n' with <br>
                    htmlText.replace("\n", "<br>");

                    // Create QLabel with styling
                    QLabel *textData = new QLabel();
                    textData->setTextFormat(Qt::RichText);
                    textData->setText(htmlText);
                    textData->setWordWrap(true);
                    textData->setStyleSheet(
                        "QLabel {"
                        "   font-size: 14px;"
                        "   color: white;"
                        "   background-color: #2e2e2e;"
                        "   padding: 12px;"
                        "   border-radius: 8px;"
                        "}"
                        );

                    // Set to scroll area
                    this->ui->scrollArea->setWidget(textData);

                    // Save query
                    QString question = this->ui->questionBox->text();
                    QString userId = this->ui->userId->text();
                    saveQuery(question, userId, responseData);
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

