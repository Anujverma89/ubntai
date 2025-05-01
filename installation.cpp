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


// installing packages
void Installation::showResultsDialog(const QStringList &packages)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle("Available Packages");
    dialog->resize(600, 400);

    QVBoxLayout *mainLayout = new QVBoxLayout(dialog);

    // Scroll area
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QWidget *scrollWidget = new QWidget;
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollWidget);


    for (const QString &line : packages) {
        QStringList parts = line.split(" - ", Qt::SkipEmptyParts);
        if (parts.isEmpty()) continue;

        QString packageName = parts[0].trimmed();
        QString description = parts.size() > 1 ? parts[1].trimmed() : "";

        QWidget *rowWidget = new QWidget;
        QHBoxLayout *rowLayout = new QHBoxLayout(rowWidget);
        rowLayout->setContentsMargins(0, 0, 0, 0);
        rowLayout->setSpacing(10);

        QLabel *label = new QLabel("<b>" + packageName + "</b>: " + description);
        label->setWordWrap(true);
        label->setFixedWidth(440);

        QPushButton *installButton = new QPushButton("Install");
        installButton->setFixedWidth(100);

        connect(installButton, &QPushButton::clicked, [=]() {
            QDialog *installDialog = new QDialog(dialog);
            installDialog->setWindowTitle("Installing: " + packageName);
            installDialog->resize(500, 300);

            QVBoxLayout *layout = new QVBoxLayout(installDialog);
            QTextEdit *outputArea = new QTextEdit;
            outputArea->setReadOnly(true);

            QPushButton *abortButton = new QPushButton("Abort");
            abortButton->setFixedWidth(80);

            layout->addWidget(new QLabel("<b>Installation Output:</b>"));
            layout->addWidget(outputArea);
            layout->addWidget(abortButton, 0, Qt::AlignRight);

            installDialog->setLayout(layout);
            installDialog->show();

            QProcess *process = new QProcess(installDialog);
            process->setProgram("pkexec");
            process->setArguments({"apt", "install", "-y", packageName});

            QObject::connect(process, &QProcess::readyReadStandardOutput, [=]() {
                QString output = QString::fromUtf8(process->readAllStandardOutput());
                outputArea->append(output.trimmed());
            });

            QObject::connect(process, &QProcess::readyReadStandardError, [=]() {
                QString error = QString::fromUtf8(process->readAllStandardError());
                outputArea->append("<font color='red'>" + error.trimmed() + "</font>");
            });

            QObject::connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                             [=](int exitCode, QProcess::ExitStatus exitStatus) {
                                 if (exitStatus == QProcess::CrashExit || exitCode != 0) {
                                    outputArea->append("<font color='red'>Installation failed or was cancelled.</font>");
                                 } else {
                                    outputArea->append("<font color='green'>Successfully installed: " + packageName + "</font>");
                                    QMessageBox::information(dialog, "Installation Successfull", QString("Successfully installed %1").arg(packageName));
                                    installDialog->close();
                                 }
                                 abortButton->setEnabled(false);
                                 process->deleteLater();
                             });

            QObject::connect(abortButton, &QPushButton::clicked, [=]() {
                if (process->state() == QProcess::Running) {
                    process->kill();
                    outputArea->append("<font color='red'>Installation aborted by user.</font>");
                }
            });

            process->start();
        });

        rowLayout->addWidget(label);
        rowLayout->addWidget(installButton);
        rowWidget->setLayout(rowLayout);
        scrollLayout->addWidget(rowWidget);
    }

    scrollWidget->setLayout(scrollLayout);
    scrollArea->setWidget(scrollWidget);

    mainLayout->addWidget(scrollArea);
    dialog->setLayout(mainLayout);
    dialog->exec();
}
