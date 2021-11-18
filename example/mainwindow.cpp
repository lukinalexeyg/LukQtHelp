#include "mainwindow.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QScreen>
#include <QPushButton>



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setFixedSize(420, 100);
    QRect rect = geometry();
    const QList<QScreen*> screens = QGuiApplication::screens();
    rect.moveCenter(screens.first()->geometry().center());
    setGeometry(rect);

    setWindowTitle(qApp->applicationName());

    m_helpViewer = new HelpViewer(qApp->applicationDirPath() + QStringLiteral("/ural_ru.qhc"), this);
    m_helpViewer->setWindowTitle(qApp->applicationName());
    m_helpViewer->setHomeSource("qthelp://ural/doc/about.html");

    m_hBoxLayout = new QHBoxLayout;

    addPushButton(QStringLiteral("Empty"), [this] { m_helpViewer->show(); });
    addPushButton(QStringLiteral("Constructor"), [this] { m_helpViewer->show(QStringLiteral("qthelp://ural/doc/constructor.html")); });
    addPushButton(QStringLiteral("Pattern"), [this] { m_helpViewer->show(QStringLiteral("qthelp://ural/doc/pattern.html")); });

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(m_hBoxLayout);
    setCentralWidget(centralWidget);
}



void MainWindow::closeEvent(QCloseEvent*)
{
    m_helpViewer->close();
}



template<typename Func>
void MainWindow::addPushButton(const QString &name, Func slot)
{
    QPushButton *pushButton = new QPushButton(name, this);

    pushButton->setFixedSize(120, 40);
    pushButton->setStyleSheet(QStringLiteral("font-size: 12pt"));

    m_hBoxLayout->addWidget(pushButton);

    connect(pushButton, &QPushButton::released, this, slot);
}
