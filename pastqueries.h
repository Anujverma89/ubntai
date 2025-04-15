#ifndef PASTQUERIES_H
#define PASTQUERIES_H

#include <QDialog>
#include <ui_mainwindow.h>
#include <QNetworkReply>
#include <QCloseEvent>


// this file is responsible for working with past quereis made by user

class Pastqueries : public QDialog
{
    Q_OBJECT;
public:
    Pastqueries(QWidget *parent = nullptr, Ui::MainWindow *ui= nullptr);
    ~Pastqueries();

    void getPastQueries();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void handleGetPastQueryReply(QNetworkReply *reply);
    void increasePageQuery();
    void decreasePageQuery();

private:
    Ui::MainWindow *ui;
    static qint64 page;
    QVBoxLayout *queryContainer;
    static QLineEdit *pageno;
    QPushButton *right;
    QPushButton *left;
};

#endif // PASTQUERIES_H
