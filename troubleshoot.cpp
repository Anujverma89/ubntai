#include "troubleshoot.h"

QDialog* Troubleshoot::dialog = nullptr;
QString Troubleshoot::filename = "Syslog.sys";
QProgressBar* Troubleshoot::progressbar = nullptr;
QLabel* Troubleshoot::readedfile= nullptr;
QLabel* Troubleshoot::fileInfo = nullptr;
QHBoxLayout *Troubleshoot::errorbox = nullptr;

Ui_MainWindow *Troubleshoot::mainui=nullptr;

Troubleshoot::Troubleshoot(Ui_MainWindow *ui) {
    mainui = ui;
}

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
    qint64 totalFiles = dir.count()-30;
    QFileInfoList files = dir.entryInfoList();

    for(int i = 0; i < totalFiles; i++){

        // logic to read file :
        QString filename = files[i].fileName();

        if(files[i].isDir()){
            continue;
        }else{
            float percent = (((i+1.0)/totalFiles)*100.0);
            progressbar->setValue(static_cast<int>(percent));
            QString updatingText = QString("Successfully read %1 of %2 Files.").arg(i+1).arg(totalFiles);
            readedfile->setText(updatingText);

            QString fileString = QString("Reading %1 file").arg(filename);
            fileInfo->setText(fileString);
            // reading file content
            if(files[i].suffix() != "gz"){
                readFileContent(files[i].filePath());
            }

            // to force ui to load the process this is not a good practice but using as of now
            QApplication::processEvents();
        }
    }
}

void Troubleshoot::showLogMessage(){
    int answer = QMessageBox::information(
        nullptr,
        "Read all log files",
        "Successfully read all files"
        );
    if(answer){
        closeReading();
    }
}

void Troubleshoot::readFileContent(QString filePath){
    QStringList errorKeywords = {
        "error", "fail", "failed", "failure", "panic", "segfault",
        "exception", "timeout", "crash", "denied", "unauthorized",
        "unreachable", "not found", "invalid", "unable to",
        "missing", "rejected", "overrun", "corrupt", "broken",
        "abort", "fatal", "halted", "conflict", "disk full",
        "kernel panic", "I/O error", "read-only", "permission denied",
        "can't", "could not", "no space left", "undefined", "hang", "assert"
    };

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning()<<"Could not open file"<<"\n";
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        for (const QString& keyword : errorKeywords) {
            if (line.contains(keyword, Qt::CaseInsensitive)) {
                Troubleshoot::populateError(line);
                break;
            }
        }
    }

    file.close();
};

void Troubleshoot::populateError(QString errorstring) {
    // Create a container widget for the scroll area if not already set
    static QWidget *scrollContent = new QWidget();
    static QVBoxLayout *scrollLayout = new QVBoxLayout(scrollContent);

    // Create a new frame for each error
    QFrame *innerFrame = new QFrame();

    innerFrame->setStyleSheet(
        "QFrame {"
        "    background-color: #444444;"
        "    border-radius: 2px;"
        "    padding: 10px;"
        "}");

    // Layout for the error and button
    QHBoxLayout *errorBox = new QHBoxLayout(innerFrame);

    // Label for the error message with word wrapping
    QLabel *unitErrorLabel = new QLabel(errorstring);
    unitErrorLabel->setWordWrap(true); // Enable word wrap
    unitErrorLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    unitErrorLabel->setStyleSheet("font-size: 14px;");

    // Troubleshoot button with styling
    QPushButton *troubleshootBtn = new QPushButton("Troubleshoot");
    troubleshootBtn->setMaximumWidth(120);
    troubleshootBtn->setStyleSheet(
        "QPushButton {"
        "    color: white;"
        "    border-radius: 2px;"
        "    padding: 8px 10px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #444444;"
        "}");

    // Add label and button to the error box layout
    errorBox->addWidget(unitErrorLabel);
    errorBox->addWidget(troubleshootBtn);

    // Set layout for the inner frame
    innerFrame->setLayout(errorBox);
    scrollLayout->addWidget(innerFrame);

    // Set layout for the scroll content
    scrollContent->setLayout(scrollLayout);

    // Set scroll area content if not already done
    mainui->troubleshootreportcontainer->setWidget(scrollContent);
    mainui->troubleshootreportcontainer->setWidgetResizable(true);
}


void Troubleshoot::closeReading(){
    dialog->close();
}
