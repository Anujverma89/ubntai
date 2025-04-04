#include "loading.h"
#include <QLabel>
#include <QMovie>
#include <QVBoxLayout>


Loading::Loading(){};

QDialog* Loading::dialog = nullptr;


void Loading::startLoading(QString context){
    dialog = new QDialog();
    QFrame *mainFrame = new QFrame();
    QVBoxLayout *layout = new QVBoxLayout(mainFrame);


    QLabel *loadingLabel = new QLabel();
    QMovie *giffy = new QMovie(":/giffy/icons/loading.gif");
    loadingLabel->setMovie(giffy);
    loadingLabel->setAlignment(Qt::AlignCenter);
    giffy->setScaledSize(QSize(60, 60));
    giffy->start();

    QLabel *text = new QLabel(context);
    //text->setAlignment(Qt::AlignCenter);
    text->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    text->setStyleSheet("font-weight:bold; font-size:16px");
    loadingLabel->setContentsMargins(0,0,0,10);
    layout->addWidget(loadingLabel);
    layout->addWidget(text);



    this->dialog->setLayout(layout);
    this->dialog->setWindowTitle("Loading ...");
    this->dialog->resize(300,110);
    this->dialog->show();
};

Loading::~Loading(){
    delete dialog;
};

void Loading::closeDialog(){
    dialog->close();
}
