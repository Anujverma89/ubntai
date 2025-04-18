#ifndef TROUBLESHOOT_H
#define TROUBLESHOOT_H

#include <QWidget>
#include <QProcess>
#include <QDialog>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QMessageBox>
#include <QTimer>
#include <QDir>
#include <QFileInfoList>
#include <QFileInfo>
#include <QDebug>
#include <QApplication>
#include "ui_mainwindow.h"
extern Ui::MainWindow *usefului;

// this file is particularly responsible for reading erros and logs and displaying them in the troubleshoot section



class Troubleshoot:QDialog
{
    Q_OBJECT
public:
    Troubleshoot(Ui_MainWindow *ui = nullptr);
    static void startTroubleShoot(QWidget *par = nullptr);
    static void readLogFiles();
    static void showLogMessage();
    static void closeReading();
    static void readFileContent(QString filePath);
    static void populateError(QString errorstring = " ");

private:
    static QDialog* dialog;
    static QString filename;
    static QProgressBar *progressbar;
    static QLabel *readedfile;
    static QLabel *fileInfo;
    static QHBoxLayout *errorbox;
    static Ui_MainWindow *mainui;
};

#endif // TROUBLESHOOT_H
