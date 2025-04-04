#include "clickablelabel.h"

ClickableLabel::ClickableLabel(QWidget *parent) : QLabel(parent) {}

void ClickableLabel::setProfileUrl(const QString &url) {
    profileUrl = url;
}

void ClickableLabel::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && !profileUrl.isEmpty()) {
        QDesktopServices::openUrl(QUrl(profileUrl));
    }
}
