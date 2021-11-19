#include "independentwindow.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QScreen>
#include <QStyle>



IndependentWindow::IndependentWindow(QWidget *parent) : QMainWindow(parent)
{
}



void IndependentWindow::setSize(QSize size, QWidget *basisWidget)
{
    const QList<QScreen*> screens = qApp->screens();
    const QRect screenRect = screens.first()->geometry();

    const int titleBarHeight = qApp->style()->pixelMetric(QStyle::PM_TitleBarHeight);

    if (size.width() < 0)
        size.setWidth(0);
    else if (size.width() > screenRect.width())
        size.setWidth(screenRect.width());

    if (size.height() < 0)
        size.setHeight(0);
    else if (size.height() > screenRect.height() - titleBarHeight)
        size.setHeight(screenRect.height() - titleBarHeight);

    if (basisWidget != nullptr) {
        int x = basisWidget->x() + (basisWidget->width() - size.width()) / 2;
        if (x < 0)
            x = 0;
        else if (x + size.width() > screenRect.width())
            x = screenRect.width() - size.width();

        int y = basisWidget->y() + (basisWidget->height() - size.height()) / 2;
        if (y < titleBarHeight)
            y = titleBarHeight;
        else if (y + size.height() > screenRect.height())
            y = screenRect.height() - size.height();

        setGeometry(x, y, size.width(), size.height());
    }

    else
        resize(size);
}



void IndependentWindow::closeEvent(QCloseEvent *event)
{
    emit closed();
    event->accept();
    deleteLater();
}
