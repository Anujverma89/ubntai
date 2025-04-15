#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateUI(QStringList links);
    void on_queryButton_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_ubntai_button_clicked();

    void on_installation_button_clicked();

    void on_troubleshoot_button_clicked();

    void on_user_button_clicked();

    void on_about_button_clicked();

    void on_pastQueries_clicked();

    void on_starttroubleshootbutton_clicked();

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
