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

    QObject::connect(troubleshootBtn, &QPushButton::clicked, [=]() {
        Troubleshoot::callGeminiAPI(errorstring);
    });

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






// void Troubleshoot::callGeminiAPI(const QString &errorText) {
//     Loading *troubleshootloader = new Loading();
//     troubleshootloader->startLoading(nullptr,"Finding solution, Please wait !!");


//     QNetworkAccessManager *manager = new QNetworkAccessManager();

//     QString apiKey = "AIzaSyBpvVYplRkFV3-XzQ0kfUnCYRnfyxIAVXM";
//     QString geminiUrl = QString("https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash:generateContent?key=%1").arg(apiKey);

//     QNetworkRequest request(geminiUrl);
//     request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

//     // Correct JSON structure
//     QString combinedText = QString(
//         "This error log contains a problem. I want you to give me three things strictly :\n"
//             "1) Cause\n"
//             "2) Solution\n"
//             "3) Version in which the solution is applicable\n"
//             "Return the result in JSON format only.\n\n"
//             "Error: %1"
//     ).arg(errorText);

//     QJsonObject textPart;
//     textPart["text"] = combinedText;


//     QJsonArray partsArray;
//     partsArray.append(textPart);

//     QJsonObject contentObject;
//     contentObject["parts"] = partsArray;

//     QJsonArray contentsArray;
//     contentsArray.append(contentObject);

//     QJsonObject json;
//     json["contents"] = contentsArray;

//     QJsonDocument jsonDoc(json);
//     QByteArray jsonData = jsonDoc.toJson();

//     // manager->post(request, jsonData);

    // QNetworkReply *reply = manager->post(request, jsonData);
    // QObject::connect(reply, &QNetworkReply::finished, [reply, troubleshootloader]() {
    //     if (reply->error() == QNetworkReply::NoError) {
    //         QByteArray response = reply->readAll();
    //         QJsonDocument responseDoc = QJsonDocument::fromJson(response);

    //         QString text = "";
    //         QJsonArray candidates = responseDoc["candidates"].toArray();
    //         if (!candidates.isEmpty()) {
    //             QJsonArray parts = candidates[0].toObject()["content"].toObject()["parts"].toArray();
    //             if (!parts.isEmpty()) {
    //                 text = parts[0].toObject()["text"].toString();
    //             }
    //         }

    //         QStringList jsonText = text.split("\n");

    //         // Print the raw Gemini response as plain text
    //         // qDebug().noquote()<<jsonText[1];
    //         // qDebug()<< "Gemini Response:\n" << jsonText;
    //         troubleshootloader->closeDialog();
    //         populateErrorResponse(jsonText[2],jsonText[3],jsonText[4]);
    //     } else {
    //         qDebug() << "Error calling Gemini API:" << reply->errorString();
    //         troubleshootloader->closeDialog();
    //     }
    //     reply->deleteLater();
    // });


// }


void Troubleshoot::callGeminiAPI(const QString &errorText) {
    Loading *troubleshootloader = new Loading();
    troubleshootloader->startLoading(nullptr,"Finding solution, Please wait !!");

    QNetworkAccessManager *manager = new QNetworkAccessManager();


    QUrl endpoint(urls::troubleShoot);
    QNetworkRequest request(endpoint);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");


    QJsonObject root;
    root["error"] = errorText;
    QJsonDocument jsonDoc(root);
    QByteArray jsonData = jsonDoc.toJson();


    QNetworkReply *reply = manager->post(request, jsonData);
    QObject::connect(reply, &QNetworkReply::finished, [reply, troubleshootloader]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            QJsonDocument responseDoc = QJsonDocument::fromJson(response);

            QString text = "";
            QJsonArray candidates = responseDoc["candidates"].toArray();
            if (!candidates.isEmpty()) {
                QJsonArray parts = candidates[0].toObject()["content"].toObject()["parts"].toArray();
                if (!parts.isEmpty()) {
                    text = parts[0].toObject()["text"].toString();
                }
            }

            QStringList jsonText = text.split("\n");


            troubleshootloader->closeDialog();
            if (jsonText.size() >= 4) {
                populateErrorResponse(jsonText[2], jsonText[3], jsonText[4]);
            } else {
                populateErrorResponse(jsonText[2], jsonText[3], jsonText[4]);
            }
        } else {
            qDebug() << "Error calling Gemini API:" << reply->errorString();
            troubleshootloader->closeDialog();
        }
        reply->deleteLater();
    });
}


// void Troubleshoot::callGeminiAPI(const QString &errorText) {
//     Loading *troubleshootloader = new Loading();
//     troubleshootloader->startLoading(nullptr, "Finding solution, please wait…");

//     QNetworkAccessManager *manager = new QNetworkAccessManager();

//     QUrl endpoint(urls::troubleShoot);
//     QNetworkRequest request(endpoint);
//     request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

//     // Build request payload
//     QJsonObject root;
//     root["error"] = errorText;
//     QJsonDocument jsonDoc(root);
//     QByteArray jsonData = jsonDoc.toJson();

//     // Fire off the POST
//     QNetworkReply *reply = manager->post(request, jsonData);
//     QObject::connect(reply, &QNetworkReply::finished, [reply, troubleshootloader]() {
//         troubleshootloader->closeDialog();

//         if (reply->error() != QNetworkReply::NoError) {
//             qDebug() << "Error calling Gemini API:" << reply->errorString();
//             reply->deleteLater();
//             return;
//         }

//         // Read and parse JSON response
//         const QByteArray response = reply->readAll();
//         const QJsonDocument responseDoc = QJsonDocument::fromJson(response);
//         const QJsonObject  obj = responseDoc.object();

//         // Extract fields
//         const QString cause    = obj.value("cause").toString(QStringLiteral("No cause provided"));
//         const QString solution = obj.value("solution").toString(QStringLiteral("No solution provided"));
//         const QString version  = obj.value("version").toString(QStringLiteral("No version provided"));

//         // Populate your UI
//         populateErrorResponse(cause, solution, version);

//         reply->deleteLater();
//     });
// }



// here we are showing the cause solution and version in which solution is applicable
void Troubleshoot::populateErrorResponse(const QString &cause, const QString &solution, const QString &version) {
    // Create a new dialog window
    QDialog *errorDialog = new QDialog();
    errorDialog->setWindowTitle("Error Resolution Details");

    // Set dark background color for the dialog
    errorDialog->setStyleSheet("background-color: #2b2b2b;");

    // Set up the layout for the dialog
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setSpacing(15);  // Add space between widgets
    layout->setContentsMargins(20, 20, 20, 20);  // Add margins

    // Create and style section headers
    QLabel *causeHeader = new QLabel("Cause:");
    QLabel *solutionHeader = new QLabel("Solution:");
    QLabel *versionHeader = new QLabel("Version:");

    QString headerStyle = "font-weight: bold; font-size: 15px; color: #ffd700;"; // golden yellow for headings
    causeHeader->setStyleSheet(headerStyle);
    solutionHeader->setStyleSheet(headerStyle);
    versionHeader->setStyleSheet(headerStyle);

    // Create labels for cause, solution, and version
    QLabel *causeLabel = new QLabel(cause);
    QLabel *solutionLabel = new QLabel(solution);
    QLabel *versionLabel = new QLabel(version);

    // Set word wrap and text styling
    QString labelStyle = "font-size: 14px; color: white;";
    causeLabel->setWordWrap(true);
    causeLabel->setStyleSheet(labelStyle);
    solutionLabel->setWordWrap(true);
    solutionLabel->setStyleSheet(labelStyle);
    versionLabel->setWordWrap(true);
    versionLabel->setStyleSheet(labelStyle);

    // Add each section to the layout
    layout->addWidget(causeHeader);
    layout->addWidget(causeLabel);
    layout->addSpacing(10);
    layout->addWidget(solutionHeader);
    layout->addWidget(solutionLabel);
    layout->addSpacing(10);
    layout->addWidget(versionHeader);
    layout->addWidget(versionLabel);
    layout->addSpacing(15);

    // Add a question prompt
    // QLabel *questionLabel = new QLabel("Would you like us to do the same for you?");
    // questionLabel->setStyleSheet("font-size: 14px; color: #ffffff;");
    // layout->addWidget(questionLabel);

    // 'Proceed' button
    QPushButton *proceedButton = new QPushButton("Ok");
    proceedButton->setStyleSheet(
        "QPushButton {"
        "    color: white;"
        "    background-color: #444444;"
        "    border-radius: 4px;"
        "    padding: 8px 10px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #5a5a5a;"
        "}"
        );

    QObject::connect(proceedButton, &QPushButton::clicked, [=]() {
        errorDialog->close();
    });

    layout->addWidget(proceedButton);

    // Set layout and display dialog
    errorDialog->setLayout(layout);
    errorDialog->exec();
}








void Troubleshoot::closeReading(){
    dialog->close();
}


