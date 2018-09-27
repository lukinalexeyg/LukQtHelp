#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("HelpViewer example");
    QApplication::setOrganizationName("Alexey Lukin");

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
