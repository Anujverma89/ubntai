#ifndef WELCOME_H
#define WELCOME_H
#include <QDialog>
#include <QLineEdit>
#include <QNetworkReply>
#include <QVBoxLayout>

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
