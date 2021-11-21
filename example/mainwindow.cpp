#include "mainwindow.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QPushButton>
#include <QScreen>
#include <QStatusBar>

static const QLatin1String s_collectionFile("Collection file: ");
static const QLatin1String s_uralFile("ural_ru.qhc");
static const QLatin1String s_invalidFile("invalid.qhc");



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setFixedSize(600, 120);

    QRect rect = geometry();
    const QList<QScreen*> screens = qApp->screens();
    rect.moveCenter(screens.first()->geometry().center());
    setGeometry(rect);

    setWindowTitle(qApp->applicationName());

    m_helpViewer = new HelpViewer(this);
    m_helpViewer->setWindowTitle(qApp->applicationName());
    m_helpViewer->setHomeSource(QStringLiteral("qthelp://ural/doc/about.html"));

    QHBoxLayout *hBoxLayout1 = new QHBoxLayout;
    QHBoxLayout *hBoxLayout2 = new QHBoxLayout;
    QVBoxLayout *vBoxLayout = new QVBoxLayout;
    vBoxLayout->addLayout(hBoxLayout1);
    vBoxLayout->addLayout(hBoxLayout2);

    QStatusBar *statusBar = new QStatusBar(this);
    setStatusBar(statusBar);
    statusBar->showMessage(s_collectionFile);

    addPushButton(hBoxLayout1, QString("Set %1 collection file").arg(s_uralFile), [this, statusBar] {
        m_helpViewer->setCollectionFile(qApp->applicationDirPath() + QLatin1Char('/') + s_uralFile);
        statusBar->showMessage(s_collectionFile + s_uralFile);
    });

    addPushButton(hBoxLayout1, QString("Set %1 collection file").arg(s_invalidFile), [this, statusBar] {
        m_helpViewer->setCollectionFile(s_invalidFile);
        statusBar->showMessage(s_collectionFile + s_invalidFile);
    });

    addPushButton(hBoxLayout2, QLatin1String("Empty"), [this] { m_helpViewer->open(); });
    addPushButton(hBoxLayout2, QLatin1String("Constructor"), [this] { m_helpViewer->open(QStringLiteral("qthelp://ural/doc/constructor.html")); });
    addPushButton(hBoxLayout2, QLatin1String("Pattern"), [this] { m_helpViewer->open(QStringLiteral("qthelp://ural/doc/pattern.html")); });
    addPushButton(hBoxLayout2, QLatin1String("Error file"), [this] { m_helpViewer->open(QStringLiteral("qthelp://ural/doc/error.html")); });

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(vBoxLayout);
    setCentralWidget(centralWidget);
}



void MainWindow::closeEvent(QCloseEvent*)
{
    m_helpViewer->close();
}



template<typename Func>
void MainWindow::addPushButton(QHBoxLayout *hBoxLayout, const QString &name, Func func)
{
    QPushButton *pushButton = new QPushButton(name, this);
    pushButton->setStyleSheet(QLatin1String("font-size: 12pt"));
    connect(pushButton, &QPushButton::released, this, func);

    hBoxLayout->addWidget(pushButton);
}
