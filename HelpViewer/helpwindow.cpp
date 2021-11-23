#include "helpwindow.h"
#include "helptoolbar.h"

#include <QGridLayout>
#include <QHelpContentWidget>
#include <QStatusBar>



HelpWindow::HelpWindow(QHelpEngine *helpEngine, QWidget *parent) :
    IndependentWindow(parent),
    m_helpEngine(helpEngine),
    m_splitterSizes{200, 600}
{
    m_helpTextBrowser = new HelpTextBrowser(helpEngine, this);

    setToolBar();
    setWidgets();
    _setStatusBar();

    connect(helpEngine->contentWidget(), &QHelpContentWidget::pressed, this, [this](const QModelIndex &modelIndex) {
        m_helpTextBrowser->setSource(m_helpEngine->contentModel()->contentItemAt(modelIndex)->url());
    });
}



void HelpWindow::setSource(const QUrl &source)
{
    m_helpTextBrowser->setSource(source);
}



void HelpWindow::setHomeSource(const QUrl &source)
{
    m_homeSource = source;
}



QUrl HelpWindow::lastSource() const
{
    return m_helpTextBrowser->historyUrl(0);
}



void HelpWindow::setToolBar()
{
    HelpToolBar *helpToolBar = new HelpToolBar(this);

    connect(helpToolBar, &HelpToolBar::contentsTriggered, this, &HelpWindow::showHideContents);
    connect(helpToolBar, &HelpToolBar::homeTriggered, this, [this]() {
        if (m_homeSource.isEmpty())
            m_helpTextBrowser->home();
        else
            m_helpTextBrowser->setSource(m_homeSource);
    });
    connect(helpToolBar, &HelpToolBar::backwardTriggered, m_helpTextBrowser, &HelpTextBrowser::backward);
    connect(helpToolBar, &HelpToolBar::forwardTriggered, m_helpTextBrowser, &HelpTextBrowser::forward);

    connect(m_helpTextBrowser, &HelpTextBrowser::backwardAvailable, helpToolBar, &HelpToolBar::setBackwardEnabled);
    connect(m_helpTextBrowser, &HelpTextBrowser::forwardAvailable, helpToolBar, &HelpToolBar::setForwardEnabled);

    addToolBar(helpToolBar);
}



void HelpWindow::setWidgets()
{
    m_splitter = new QSplitter(Qt::Horizontal);
    m_splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_splitter->addWidget(m_helpEngine->contentWidget());
    m_splitter->addWidget(m_helpTextBrowser);

    m_splitter->setStretchFactor(0, 0);
    m_splitter->setStretchFactor(1, 1);

    m_splitter->setCollapsible(0, true);
    m_splitter->setCollapsible(1, false);

    m_splitter->setSizes(m_splitterSizes);

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->setAlignment(Qt::AlignCenter);
    gridLayout->setContentsMargins(5, 5, 5, 0);
    gridLayout->addWidget(m_splitter, 0, 0);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(gridLayout);
    setCentralWidget(centralWidget);
}



void HelpWindow::_setStatusBar()
{
    m_statusBar = new QStatusBar(this);

    connect(m_helpTextBrowser, QOverload<const QUrl &>::of(&QTextBrowser::highlighted), this, [this](const QUrl &link) {
        m_statusBar->showMessage(link.toString());
    });

    setStatusBar(m_statusBar);
}



void HelpWindow::showHideContents()
{
    if (m_helpEngine->contentWidget()->width() > 0) {
        m_splitterSizes = m_splitter->sizes();
        QList<int> newHorizontalSizes;
        newHorizontalSizes << 0 << 800;
        m_splitter->setSizes(newHorizontalSizes);
    }
    else
        m_splitter->setSizes(m_splitterSizes);
}
