#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName(APP_PRODUCT);

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
