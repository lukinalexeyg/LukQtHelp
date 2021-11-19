#include "helpwindow.h"
#include "helptoolbar.h"

#include <QGridLayout>
#include <QHelpContentWidget>
#include <QStatusBar>
#include <QToolBar>



HelpWindow::HelpWindow(QHelpEngine *helpEngine, QWidget *parent) :
    IndependentWindow(parent),
    m_helpEngine(helpEngine)
{
    m_helpTextBrowser = new HelpTextBrowser(helpEngine, this);
    m_helpContentModel = helpEngine->contentModel();

    setToolBar();
    setContent();

    m_statusBar = new QStatusBar(this);
    setStatusBar(m_statusBar);

    connect(helpEngine->contentWidget(), &QHelpContentWidget::pressed, this, [this](const QModelIndex &modelIndex) {
        m_helpTextBrowser->setSource(m_helpContentModel->contentItemAt(modelIndex)->url());
    });

    connect(m_helpTextBrowser, QOverload<const QUrl &>::of(&QTextBrowser::highlighted), this, [this](const QUrl &link) {
        m_statusBar->showMessage(link.toString());
    });
}



void HelpWindow::setSource(const QUrl &source)
{
    m_helpTextBrowser->setSource(source);
}



void HelpWindow::setHomeSource(const QUrl &source)
{
    if (!m_helpEngine->fileData(source).isEmpty())
        m_homeSource = source;
}



QUrl HelpWindow::lastSource() const
{
    if (m_helpTextBrowser != nullptr)
        return m_helpTextBrowser->historyUrl(0);

    return QUrl();
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



void HelpWindow::setContent()
{
    m_horizontalSplitter = new QSplitter(Qt::Horizontal);
    m_horizontalSplitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_horizontalSplitter->addWidget(m_helpEngine->contentWidget());
    m_horizontalSplitter->addWidget(m_helpTextBrowser);

    m_horizontalSplitter->setStretchFactor(0, 0);
    m_horizontalSplitter->setStretchFactor(1, 1);

    m_horizontalSplitter->setCollapsible(0, true);
    m_horizontalSplitter->setCollapsible(1, false);

    m_horizontalSplitter->setSizes(m_horizontalSplitterSizes);

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->setAlignment(Qt::AlignCenter);
    gridLayout->setContentsMargins(5, 5, 5, 0);
    gridLayout->addWidget(m_horizontalSplitter, 0, 0);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(gridLayout);
    setCentralWidget(centralWidget);
}



void HelpWindow::showHideContents()
{
    if (m_helpEngine->contentWidget()->width() > 0) {
        m_horizontalSplitterSizes = m_horizontalSplitter->sizes();
        QList<int> newHorizontalSizes;
        newHorizontalSizes << 0 << 800;
        m_horizontalSplitter->setSizes(newHorizontalSizes);
    }
    else
        m_horizontalSplitter->setSizes(m_horizontalSplitterSizes);
}
