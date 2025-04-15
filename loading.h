#ifndef LOADING_H
#define LOADING_H
#include <QObject>
#include <QDialog>
#include <QWidget>

// this is basically a loader which pops up when some computation is going on behind the scens
class Loading : public QDialog
{
    Q_OBJECT
public:
    Loading();
    ~Loading();
    void  startLoading(QWidget *parent = nullptr, QString context="");
    void  closeDialog();

private:
    static QDialog *dialog;
};


#endif // LOADING_H
