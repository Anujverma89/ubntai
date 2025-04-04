#ifndef ASISTANT_H
#define ASISTANT_H
#include <QNetworkReply>
#include<ui_mainwindow.h>
#include <QObject>
class Asistant : public QObject
{
    Q_OBJECT;
public:
    Asistant(Ui::MainWindow *ui);
    ~Asistant();

    void makeRequest(Ui::MainWindow *ui);
    void saveQuery(QString question, QString userid, QString answer);

private slots:
    void handleResponse(QNetworkReply *reply);
    void handleSaveQuery(QNetworkReply *reply);


private :
    Ui::MainWindow *ui;

};

#endif // ASISTANT_H

// void makeRequest(Ui:MainWindow *Ui);
// user clicks on go button
// we take a question from ui so we will need a ui
// we will load a loader window and close when the request is finished
// we will make a request to the Gemini api
// geimin will return a response
// again we will make a request to save that response to the user database in mongod
// after this we will show the response in the ui;


// once we start generating the code
//
