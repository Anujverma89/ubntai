#ifndef LOADING_H
#define LOADING_H
#include <QObject>
#include <QDialog>
#include <QWidget>
class Loading : public QDialog
{
public:
    Loading();
    ~Loading();
    void  startLoading(QString context="");
    void  closeDialog();

private:
    static QDialog *dialog;
};


#endif // LOADING_H
