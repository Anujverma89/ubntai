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

class Installation : QObject
{
    Q_OBJECT;
public:
    Installation();
    static void showInstalledApps(Ui::MainWindow *ui);
};

#endif // INSTALLATION_H
