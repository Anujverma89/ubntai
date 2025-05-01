#ifndef INSTALLATION_H
#define INSTALLATION_H
#include <QWidget>
#include <QObject>
#include <ui_mainwindow.h>
#include <QComboBox>
#include <QDir>
#include <QVBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QDialog>
#include <QMessageBox>
#include <QProcess>

class Installation : QObject
{
    Q_OBJECT;
public:
    Installation();
    static void showResultsDialog(const QStringList &packages);
    static void showInstalledApps(Ui::MainWindow *ui);
};

#endif // INSTALLATION_H
