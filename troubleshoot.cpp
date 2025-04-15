#include "troubleshoot.h"

QDialog* Troubleshoot::dialog = nullptr;
QString Troubleshoot::filename = "Syslog.sys";
QProgressBar* Troubleshoot::progressbar = nullptr;
QLabel* Troubleshoot::readedfile= nullptr;
QLabel* Troubleshoot::fileInfo = nullptr;

Troubleshoot::Troubleshoot() {}

void Troubleshoot::startTroubleShoot(QWidget *par){
    dialog = new QDialog(par);
    QFrame *mainframe = new QFrame();
    QVBoxLayout *verticalLayout = new QVBoxLayout(mainframe);

    readedfile = new QLabel("");
    fileInfo = new QLabel("");
    progressbar =  new QProgressBar();



    //styling
    readedfile->setAlignment(Qt::AlignCenter);
    readedfile->setContentsMargins(0,0,0,5);
    fileInfo->setAlignment(Qt::AlignCenter);
    fileInfo->setContentsMargins(0,20,0,0);
    progressbar->setRange(0,100);


    //adding into UI
    verticalLayout->addWidget(readedfile);
    verticalLayout->addWidget(progressbar);
    verticalLayout->addWidget(fileInfo);
    verticalLayout->setAlignment(Qt::AlignCenter | Qt::AlignTop);
    verticalLayout->setContentsMargins(20,20,20,20);
    verticalLayout->setSpacing(5);


    // dialog setting & configuration
    dialog->setLayout(verticalLayout);
    dialog->setWindowTitle("Reading log files...");
    dialog->setModal(true);
    dialog->resize(350,150);
    dialog->show();

    readLogFiles();
    showLogMessage();
}

void Troubleshoot::readLogFiles() {
    QDir dir("/var/log");
    // -2 becuase one for forward . and backward directory ..
    qint64 totalFiles = dir.count();
    QFileInfoList files = dir.entryInfoList();
    for(int i = 0; i < totalFiles; i++){

        // logic to read file :
        QString filename = files[i].fileName();

        if(files[i].isDir()){
            continue;
        }else{
            // reading file content
            readFileContent(files[i].filePath());

            float percent = (((i+1.0)/totalFiles)*100.0);
            progressbar->setValue(static_cast<int>(percent));
            QString updatingText = QString("Successfully read %1 of %2 Files.").arg(i+1).arg(totalFiles);
            readedfile->setText(updatingText);

            QString fileString = QString("Reading %1th file").arg(filename);
            fileInfo->setText(fileString);
        }
    }
}

void Troubleshoot::showLogMessage(){
    int answer = QMessageBox::information(
        nullptr,                    // Parent (can be `this` or any QWidget*)
        "Read all log files",                       // Title
        "Successfully read all files"  // Message
        );
    if(answer){
        closeReading();
    }
}

void Troubleshoot::readFileContent(QString filePath){
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Can't open file:" << filePath;
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
    }

    file.close();
};



void Troubleshoot::closeReading(){
    dialog->close();
}
