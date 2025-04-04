#ifndef NETWORKING_H
#define NETWORKING_H
#include <QString>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QStringList>

// this file deals with network calls realted to the Aabout page

class Networking : public QObject
{
    Q_OBJECT;

public:
    Networking();
    ~Networking();
    QStringList getHostIp();
    QNetworkAccessManager *manager;
    QStringList links;


private slots:
    void handleResponse(QNetworkReply *reply);




signals:
    void dataReceived(QStringList links);

private:

};

#endif // NETWORKING_H
