#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName(QStringLiteral("LukQtHelpExample"));
    QApplication::setOrganizationName(QStringLiteral("Alexey Lukin"));

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
