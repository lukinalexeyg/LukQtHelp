#include <QHelpContentWidget>
#include <QToolBar>
#include <QStatusBar>
#include <QGridLayout>
#include "helpwindow.h"



HelpWindow::HelpWindow(QHelpEngine *helpEngine, QWidget *parent) :
    IndependentWindow(parent),
    helpEngine(helpEngine)
{
    helpTextBrowser = new HelpTextBrowser(helpEngine, this);
    helpContentModel = helpEngine->contentModel();

    setToolBar();
    retranslate();
    setContent();

    statusBar = new QStatusBar(this);
    setStatusBar(statusBar);

    connect(helpEngine->contentWidget(), &QHelpContentWidget::pressed, this, [this](const QModelIndex &modelIndex){
        helpTextBrowser->setSource(helpContentModel->contentItemAt(modelIndex)->url());
    });

    connect(helpTextBrowser, QOverload<const QString &>::of(&QTextBrowser::highlighted),
            [=](const QString &link){statusBar->showMessage(link);});
}



void HelpWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
        retranslate();
    IndependentWindow::changeEvent(event);
}



void HelpWindow::retranslate()
{
    showHideContentsAction->setText(tr("Show/hide content"));
    homeContentAction->setText(tr("Home"));
    backContentAction->setText(tr("Back"));
    forwardContentAction->setText(tr("Forward"));
}



void HelpWindow::setSource(const QString &source)
{
    helpTextBrowser->setSource(QUrl(source));
}



void HelpWindow::setHomeSource(const QString &source)
{
    if (!helpEngine->fileData(QUrl(source)).isEmpty())
        homeSource = QUrl(source);
}



QString HelpWindow::lastSource()
{
    if (helpTextBrowser != Q_NULLPTR)
        return helpTextBrowser->historyUrl(0).toString();

    return "";
}



void HelpWindow::setToolBar()
{
    QToolBar *toolBar = new QToolBar(this);
    toolBar->setFloatable(false);
    toolBar->setMovable(false);
    toolBar->setContextMenuPolicy(Qt::PreventContextMenu);
    toolBar->setStyleSheet("QToolBar {padding: 5}"
                           "QToolBar QToolButton {padding: 2; margin: 0}");
    toolBar->setWindowTitle(tr("Toolbar"));

    showHideContentsAction = toolBar->addAction(QIcon(":/images/icon-menu.svg"), "", this, &HelpWindow::showHideContents);

    toolBar->addSeparator();

    homeContentAction = toolBar->addAction(QIcon(":/images/icon-home.svg"), "", helpTextBrowser, [this](){
        if (homeSource.isEmpty())
            helpTextBrowser->home();
        else
            helpTextBrowser->setSource(homeSource);
    });

    backContentAction = toolBar->addAction(QIcon(":/images/icon-arrow-left.svg"), "", helpTextBrowser, &HelpTextBrowser::backward);
    forwardContentAction = toolBar->addAction(QIcon(":/images/icon-arrow-right.svg"), "", helpTextBrowser, &HelpTextBrowser::forward);

    connect(helpTextBrowser, &HelpTextBrowser::backwardAvailable, backContentAction, &QAction::setEnabled);
    connect(helpTextBrowser, &HelpTextBrowser::forwardAvailable, forwardContentAction, &QAction::setEnabled);

    showHideContentsAction->setShortcut(Qt::Key_F2);
    backContentAction->setShortcut(QKeySequence::MoveToPreviousPage);
    homeContentAction->setShortcut(QKeySequence::MoveToStartOfLine);
    forwardContentAction->setShortcut(QKeySequence::MoveToNextPage);

    addToolBar(toolBar);
}



void HelpWindow::setContent()
{
    horizontalSplitter = new QSplitter(Qt::Horizontal);
    horizontalSplitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    horizontalSplitter->addWidget(helpEngine->contentWidget());
    horizontalSplitter->addWidget(helpTextBrowser);

    horizontalSplitter->setStretchFactor(0, 0);
    horizontalSplitter->setStretchFactor(1, 1);

    horizontalSplitter->setCollapsible(0, true);
    horizontalSplitter->setCollapsible(1, false);

    horizontalSplitter->setSizes(m_horizontalSplitterSizes);

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->setAlignment(Qt::AlignCenter);
    gridLayout->setContentsMargins(5, 5, 5, 0);
    gridLayout->addWidget(horizontalSplitter, 0, 0);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(gridLayout);
    setCentralWidget(centralWidget);
}



void HelpWindow::showHideContents()
{
    if (helpEngine->contentWidget()->width() > 0) {
        m_horizontalSplitterSizes = horizontalSplitter->sizes();
        QList<int> newHorizontalSizes;
        newHorizontalSizes << 0 << 800;
        horizontalSplitter->setSizes(newHorizontalSizes);
    }
    else
        horizontalSplitter->setSizes(m_horizontalSplitterSizes);
}
