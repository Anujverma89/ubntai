#include "networking.h"
#include <QHostAddress>
#include <QHostInfo>
#include <QNetworkAccessManager>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QStringList>
#include <QNetworkInterface>
#include <QPainter>
#include <QLabel>
#include <QPixmap>
#include <QPainterPath>


Networking::Networking(){
    this->manager = new QNetworkAccessManager();
    connect(this->manager, &QNetworkAccessManager::finished, this, &Networking::handleResponse);
};
Networking::~Networking(){
    delete this->manager;
};

QStringList Networking::getHostIp(){
    QString hostName = QHostInfo::localHostName();
    QHostInfo hostInfo = QHostInfo::fromName(hostName);
    QStringList ipadd;

    for (const QHostAddress &address : hostInfo.addresses()) {
        ipadd.push_back(address.toString());
    }

    // Fallback: If still empty, get IP from network interfaces
    if (ipadd.isEmpty()) {
        for (const QNetworkInterface &netInterface : QNetworkInterface::allInterfaces()) {
            for (const QNetworkAddressEntry &entry : netInterface.addressEntries()) {
                if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol &&
                    entry.ip() != QHostAddress::LocalHost) {
                    ipadd.push_back(entry.ip().toString());
                }
            }
            if (!ipadd.isEmpty()) break;
        }
    }
    return ipadd;
}

void Networking::handleResponse(QNetworkReply *reply){
    if (reply->error() == QNetworkReply::NoError) {
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        QJsonArray jsonArray = doc.array();
        for (const QJsonValue &value : jsonArray) {
            QJsonObject obj = value.toObject();
            QString avatarUrl = obj["avatar_url"].toString();
            QString htmlUrl = obj["html_url"].toString();
            this->links.push_back(avatarUrl);
            this->links.push_back(htmlUrl);
        }
        emit dataReceived(links);
    } else {
        qDebug() << "Error:" << reply->errorString();
    }
    reply->deleteLater();
}



