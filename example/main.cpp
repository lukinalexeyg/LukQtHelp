#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("HelpViewerExample");
    QApplication::setOrganizationName("Alexey Lukin");

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
