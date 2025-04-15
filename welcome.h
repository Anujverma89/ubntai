#ifndef WELCOME_H
#define WELCOME_H
#include <QDialog>
#include <QLineEdit>
#include <QNetworkReply>
#include <QVBoxLayout>

//this file basically does initialization of the app when it is installed for the first time in the system
// feteches the already registered user or create new user if user doesnot exists

class Welcome : public QDialog
{
    Q_OBJECT;
public:
    Welcome(QWidget *parent = nullptr);
    ~Welcome();

private slots:
    void onNextButtonClicked();
    void closeWindow();
    void handleReply(QNetworkReply *reply);

private :
    QLineEdit *email;
};

#endif // WELCOME_H
