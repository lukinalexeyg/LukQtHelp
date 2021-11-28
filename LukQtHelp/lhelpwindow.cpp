#include "lhelpwindow.h"
#include "lhelptoolbar.h"

#include <QGridLayout>
#include <QHelpContentWidget>
#include <QStatusBar>



LHelpWindow::LHelpWindow(QHelpEngine *helpEngine, QWidget *parent) :
    LStandaloneMainWindow(parent),
    m_helpEngine(helpEngine),
    m_splitterSizes{200, 600}
{
    m_helpTextBrowser = new LHelpTextBrowser(helpEngine, this);

    setToolBar();
    setWidgets();
    _setStatusBar();

    connect(helpEngine->contentWidget(), &QHelpContentWidget::pressed, this, &LHelpWindow::onContentWidgetPressed);
}



void LHelpWindow::setSource(const QUrl &source)
{
    m_helpTextBrowser->setSource(source);
}



QUrl LHelpWindow::lastSource() const
{
    return m_helpTextBrowser->historyUrl(0);
}



void LHelpWindow::setExternalLinksEnabled(const bool enabled)
{
    m_helpTextBrowser->setExternalLinksEnabled(enabled);
}



bool LHelpWindow::externalLinksEnabled() const
{
    return m_helpTextBrowser->externalLinksEnabled();
}



void LHelpWindow::setSplitterSizes(const QList<int> &sizes)
{
    m_splitter->setSizes(sizes);
}



QList<int> LHelpWindow::splitterSizes() const
{
    return m_splitter->sizes();
}



void LHelpWindow::setToolBar()
{
    LHelpToolBar *helpToolBar = new LHelpToolBar(this);

    connect(helpToolBar, &LHelpToolBar::contentsTriggered, this, &LHelpWindow::showHideContents);
    connect(helpToolBar, &LHelpToolBar::homeTriggered, this, &LHelpWindow::onToolBarHomeTriggered);
    connect(helpToolBar, &LHelpToolBar::backwardTriggered, m_helpTextBrowser, &LHelpTextBrowser::backward);
    connect(helpToolBar, &LHelpToolBar::forwardTriggered, m_helpTextBrowser, &LHelpTextBrowser::forward);

    connect(m_helpTextBrowser, &LHelpTextBrowser::backwardAvailable, helpToolBar, &LHelpToolBar::setBackwardEnabled);
    connect(m_helpTextBrowser, &LHelpTextBrowser::forwardAvailable, helpToolBar, &LHelpToolBar::setForwardEnabled);

    addToolBar(helpToolBar);
}



void LHelpWindow::setWidgets()
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



void LHelpWindow::_setStatusBar()
{
    m_statusBar = new QStatusBar(this);

    connect(m_helpTextBrowser, QOverload<const QUrl &>::of(&QTextBrowser::highlighted), this, &LHelpWindow::onTextBrowserHighlighted);

    setStatusBar(m_statusBar);
}



void LHelpWindow::showHideContents()
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



void LHelpWindow::onContentWidgetPressed(const QModelIndex &modelIndex)
{
    const QUrl url = m_helpEngine->contentModel()->contentItemAt(modelIndex)->url();
    m_helpTextBrowser->setSource(url);
}



void LHelpWindow::onToolBarHomeTriggered()
{
    if (m_homeSource.isEmpty())
        m_helpTextBrowser->home();
    else
        m_helpTextBrowser->setSource(m_homeSource);
}



void LHelpWindow::onTextBrowserHighlighted(const QUrl &link)
{
    m_statusBar->showMessage(link.toString());
}
