#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>
#include <QUrl>
#include <QDesktopServices>
#include <QMouseEvent>


// this file is used for clickable icon on the about page that redirects to the contributors link

class ClickableLabel : public QLabel {
    Q_OBJECT

public:
    explicit ClickableLabel(QWidget *parent = nullptr);
    void setProfileUrl(const QString &url);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    QString profileUrl;
};

#endif // CLICKABLELABEL_H
