#include "installation.h"
#include "flowlayout.h"

Installation::Installation() {}

void Installation::showInstalledApps(Ui::MainWindow *ui){
    qint16 filter = ui->filterbox->currentIndex();
    if(filter == 0 ){
        // fetch gui applications
        QDir *snapdir = new QDir("/snap");
        QDir *flatpak = new QDir("/var/lib/flatpak");

        qint64 totalsnapdir = snapdir->count();
        qint64 totalflatpak = flatpak->count();


        QFileInfoList snapdirinfo = snapdir->entryInfoList();
        QFileInfoList flatpakinfo = flatpak->entryInfoList();


        QScrollArea *installedArea = ui->installedappscrollarea;
        QLineEdit *installedpackge = ui->Installedpackge;
        installedpackge->setText(QString("Installed Packages : %1").arg((totalsnapdir-2)+(totalflatpak-2)));

        QWidget *container = new QWidget(installedArea);
        FlowLayout *flowLayout = new FlowLayout(container, 5, 10, 10);


        // Iterate over binaries
        for (int i = 2; i < totalsnapdir; i++) {
            QLabel *label = new QLabel(snapdirinfo[i].fileName());
            label->setStyleSheet(R"(
                    QLabel {
                        padding: 8px;
                        background-color: #333333;
                        border-radius: 2px;
                        color: white;
                        min-height: 40px;
                        min-width:200px;
                        max-width: 200px;
                    }
                    QLabel:hover {
                        background-color: #444444;
                    }
                )");

            flowLayout->addWidget(label);
        }

        for (int i = 2; i < totalflatpak; i++) {
            QLabel *label = new QLabel(flatpakinfo[i].fileName());
            label->setStyleSheet(R"(
                    QLabel {
                        padding: 8px;
                        background-color: #333333;
                        border-radius: 2px;
                        color: white;
                        min-height: 40px;
                        min-width:200px;
                        max-width: 200px;
                    }
                    QLabel:hover {
                        background-color: #444444;
                    }
                )");

            flowLayout->addWidget(label);
        }

        container->setLayout(flowLayout);
        installedArea->setWidget(container);
        installedArea->setWidgetResizable(true);


    }else if(filter == 1){
        // fetch cli apps
        QDir *exectuables = new QDir("/bin");
        qint64 totalbinaries = exectuables->count();
        QFileInfoList fileinfo = exectuables->entryInfoList();
        QScrollArea *installedArea = ui->installedappscrollarea;
        QLineEdit *installedpackge = ui->Installedpackge;
        installedpackge->setText(QString("Installed Packages : %1").arg(totalbinaries-2));

        QWidget *container = new QWidget(installedArea);
        FlowLayout *flowLayout = new FlowLayout(container, 5, 10, 10);


        // Iterate over binaries
        for (int i = 3; i < totalbinaries; i++) {
            QLabel *label = new QLabel(fileinfo[i].fileName());
                    label->setStyleSheet(R"(
                    QLabel {
                        padding: 8px;
                        background-color: #333333;
                        border-radius: 2px;
                        color: white;
                        min-height: 40px;
                        min-width:200px;
                        max-width: 200px;
                    }
                    QLabel:hover {
                        background-color: #444444;
                    }
                )");

            flowLayout->addWidget(label);
        }

        container->setLayout(flowLayout);
        installedArea->setWidget(container);
        installedArea->setWidgetResizable(true);

    }else{
        // case when the filter 2 fetch libraries
        QDir *mainlib = new QDir("/lib");
        QDir *usrshare = new QDir("/usr/share");


        qint64 totalmainlib = mainlib->count();
        qint64 totalusrshare = usrshare->count();


        QFileInfoList mainlibinfo = mainlib->entryInfoList();
        QFileInfoList usrshareinfo = usrshare->entryInfoList();


        QScrollArea *installedArea = ui->installedappscrollarea;
        QLineEdit *installedpackge = ui->Installedpackge;
        installedpackge->setText(QString("Installed Packages : %1").arg((totalmainlib-2)+(totalusrshare-2)));

        QWidget *container = new QWidget(installedArea);
        FlowLayout *flowLayout = new FlowLayout(container, 5, 10, 10);


        // Iterate over binaries
        for (int i = 2; i < totalmainlib; i++) {
            QLabel *label = new QLabel(mainlibinfo[i].fileName());
            label->setStyleSheet(R"(
                    QLabel {
                        padding: 8px;
                        background-color: #333333;
                        border-radius: 2px;
                        color: white;
                        min-height: 40px;
                        min-width:200px;
                        max-width: 200px;
                    }
                    QLabel:hover {
                        background-color: #444444;
                    }
                )");

            flowLayout->addWidget(label);
        }

        for (int i = 2; i < totalusrshare; i++) {
            QLabel *label = new QLabel(usrshareinfo[i].fileName());
            label->setStyleSheet(R"(
                    QLabel {
                        padding: 8px;
                        background-color: #333333;
                        border-radius: 2px;
                        color: white;
                        min-height: 40px;
                        min-width:200px;
                        max-width: 200px;
                    }
                    QLabel:hover {
                        background-color: #444444;
                    }
                )");

            flowLayout->addWidget(label);
        }

        container->setLayout(flowLayout);
        installedArea->setWidget(container);
        installedArea->setWidgetResizable(true);

    }
};
