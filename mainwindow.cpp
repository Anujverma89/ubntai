#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "networking.h"
#include <QSysInfo>
#include <QDebug>
#include <QProcess>
#include <QNetworkRequest>
#include <QDesktopServices>
#include <QPainter>
#include <QLabel>
#include <QPixmap>
#include <QPainterPath>
#include "clickablelabel.h"
#include "welcome.h"
#include <QTimer>
#include "db.h"
#include "user.h"
#include "asistant.h"
#include "pastqueries.h"
#include <QProgressBar>
#include "loading.h"
#include "troubleshoot.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Db db;
    db.initializeDatabase();
    User user;
    // for first launch it's nothing for other launches it 0
    int isfirstlaunch =  user.isFirstLaunch();


    if(isfirstlaunch == 1){
        // loading Welcome dialog after second 1 second
        Welcome *window = new Welcome(this);
        QTimer::singleShot(500, [this]() {
            Welcome *window = new Welcome(this);
            window->exec();
            window->deleteLater(); // Ensures cleanup after dialog closes
            ui->user_button->click();
        });
    }

    user.getUser(ui);

    // we will be calling a get user to show details on UI

    QString sysName = QSysInfo::prettyProductName();
    QString kernalType = QSysInfo::kernelType();
    QString kernalVersion = QSysInfo::kernelVersion();
    QString archType = QSysInfo::currentCpuArchitecture();
    QString host = QSysInfo::machineHostName();
    QString formatted = QString("%1\n %2\n %3\n %4\n %5\n")
                            .arg(sysName)
                            .arg(kernalType)
                            .arg(kernalVersion)
                            .arg(archType)
                            .arg(host);
    ui->informationText->setText(formatted);

}


void MainWindow::updateUI(QStringList links) {
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(ui->contributorsList->layout());

    // If no layout exists, create one
    if (!layout) {
        layout = new QVBoxLayout(ui->contributorsList);
        ui->contributorsList->setLayout(layout);
    }

    // Clear previous items
    while (QLayoutItem *item = layout->takeAt(0)) {
        delete item->widget();
        delete item;
    }

    for (int i = 0; i < links.size(); i += 2) {
        QString avatarUrl = links[i];       // Image URL
        QString profileUrl = links[i + 1];  // GitHub profile URL

        ClickableLabel *imageLabel = new ClickableLabel();
        imageLabel->setCursor(Qt::PointingHandCursor); // Hand cursor on hover
        imageLabel->setProfileUrl(profileUrl);

        // Load image asynchronously
        QNetworkAccessManager *manager = new QNetworkAccessManager(imageLabel);
        connect(manager, &QNetworkAccessManager::finished, [imageLabel](QNetworkReply *reply) {
            if (reply->error() == QNetworkReply::NoError) {
                QPixmap pixmap;
                pixmap.loadFromData(reply->readAll());

                // Convert to 40x40 circular image
                QPixmap circularPixmap(40, 40);
                circularPixmap.fill(Qt::transparent);

                QPainter painter(&circularPixmap);
                painter.setRenderHint(QPainter::Antialiasing, true);
                painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

                QPainterPath path;
                path.addEllipse(0, 0, 40, 40);
                painter.setClipPath(path);
                painter.drawPixmap(0, 0, 40, 40, pixmap);

                imageLabel->setPixmap(circularPixmap);
            }
            reply->deleteLater();
        });

        manager->get(QNetworkRequest(QUrl(avatarUrl)));

        layout->addWidget(imageLabel);
    }
    ui->stackedWidget->setCurrentIndex(4);
}

MainWindow::~MainWindow()
{
    delete ui;
}


//this function request's the answer from gemini
void MainWindow::on_queryButton_clicked()
{
    QString innertext = ui->queryBox->text();
    ui->questionBox->setText(innertext);
    Loading *loader = new Loading();
    loader->startLoading(this, "Making request, Please Wait !!");
    Asistant *asistant = new Asistant(ui);
    asistant->makeRequest(ui);
}


void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    if(index==0){
        QString sysName = QSysInfo::prettyProductName();
        QString kernalType = QSysInfo::kernelType();
        QString kernalVersion = QSysInfo::kernelVersion();
        QString archType = QSysInfo::currentCpuArchitecture();
        QString host = QSysInfo::machineHostName();
        QString formatted = QString("%1\n %2\n %3\n %4\n %5\n")
                                .arg(sysName)
                                .arg(kernalType)
                                .arg(kernalVersion)
                                .arg(archType)
                                .arg(host);
        ui->informationText->setText(formatted);

    }else if(index == 1){
        Networking hostadd;
        QStringList ipadd = hostadd.getHostIp();
        ui->informationText->setText("");
        for (const QString &ip : ipadd) {
            qDebug()<<ip<<"\n";
            ui->informationText->setText(ip);
        }


    }else if(index == 2){

    }else{
        qDebug()<<"Index out of Bound";
    }
}



void MainWindow::on_ubntai_button_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_installation_button_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_troubleshoot_button_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}


void MainWindow::on_user_button_clicked()
{
    User user;
    user.getUser(ui);
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_about_button_clicked()
{
    Networking *network = new Networking();
    connect(network, &Networking::dataReceived, this, &MainWindow::updateUI);
    QNetworkRequest request(QUrl("https://api.github.com/repos/AnujVerma89/ubntai/contributors"));
    network->manager->get(request);
    ui->stackedWidget->setCurrentIndex(4);
}







void MainWindow::on_pastQueries_clicked()
{
    Pastqueries *pastqueries = new Pastqueries(nullptr,ui);
    pastqueries->show();
}


void MainWindow::on_starttroubleshootbutton_clicked()
{
    Troubleshoot::startTroubleShoot(this);
}

