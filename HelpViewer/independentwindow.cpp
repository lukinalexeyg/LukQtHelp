#include "independentwindow.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QScreen>
#include <QStyle>



IndependentWindow::IndependentWindow(QWidget *parent) : QMainWindow(parent)
{
}



void IndependentWindow::setSize(const QSize &size, QWidget *basisWidget)
{
    QSize s = size;
    const QList<QScreen*> screens = qApp->screens();
    const QRect screenRect = screens.first()->geometry();
    const int titleBarHeight = qApp->style()->pixelMetric(QStyle::PM_TitleBarHeight);

    s.setWidth(qBound(0, s.width(), screenRect.width()));
    s.setHeight(qBound(0, s.height(), screenRect.height() - titleBarHeight));

    if (basisWidget != nullptr) {
        int x = basisWidget->x() + (basisWidget->width() - s.width()) / 2;
        if (x < 0)
            x = 0;
        else if (x + s.width() > screenRect.width())
            x = screenRect.width() - s.width();

        int y = basisWidget->y() + (basisWidget->height() - s.height()) / 2;
        if (y < titleBarHeight)
            y = titleBarHeight;
        else if (y + s.height() > screenRect.height())
            y = screenRect.height() - s.height();

        setGeometry(x, y, s.width(), s.height());
    }

    else
        resize(s);
}



void IndependentWindow::closeEvent(QCloseEvent *event)
{
    emit closed();
    event->accept();
    delete this;
}
