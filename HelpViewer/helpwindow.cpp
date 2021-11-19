#include "helpwindow.h"

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
    retranslate();
    setContent();

    m_statusBar = new QStatusBar(this);
    setStatusBar(m_statusBar);

    connect(helpEngine->contentWidget(), &QHelpContentWidget::pressed, this, [this](const QModelIndex &modelIndex) {
        m_helpTextBrowser->setSource(m_helpContentModel->contentItemAt(modelIndex)->url());
    });

    connect(m_helpTextBrowser, QOverload<const QUrl &>::of(&QTextBrowser::highlighted), this, [this](const QUrl &link) {
        m_statusBar->showMessage(link.toString()); });
}



void HelpWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
        retranslate();
    IndependentWindow::changeEvent(event);
}



void HelpWindow::retranslate()
{
    m_showHideContentsAction->setText(tr("Show/hide content"));
    m_homeContentAction->setText(tr("Home"));
    m_backContentAction->setText(tr("Back"));
    m_forwardContentAction->setText(tr("Forward"));
}



void HelpWindow::setSource(const QString &source)
{
    m_helpTextBrowser->setSource(QUrl(source));
}



void HelpWindow::setHomeSource(const QString &source)
{
    if (!m_helpEngine->fileData(QUrl(source)).isEmpty())
        m_homeSource = QUrl(source);
}



QString HelpWindow::lastSource() const
{
    if (m_helpTextBrowser != nullptr)
        return m_helpTextBrowser->historyUrl(0).toString();

    return QString();
}



void HelpWindow::setToolBar()
{
    QToolBar *toolBar = new QToolBar(this);
    toolBar->setFloatable(false);
    toolBar->setMovable(false);
    toolBar->setContextMenuPolicy(Qt::PreventContextMenu);
    toolBar->setStyleSheet(QStringLiteral("QToolBar {padding: 5}"
                                          "QToolBar QToolButton {padding: 2; margin: 0}"));
    toolBar->setWindowTitle(tr("Toolbar"));

    m_showHideContentsAction = toolBar->addAction(QIcon(QStringLiteral(":/images/icon-menu.svg")), QString(),
                                                  this, &HelpWindow::showHideContents);

    toolBar->addSeparator();

    m_homeContentAction = toolBar->addAction(QIcon(QStringLiteral(":/images/icon-home.svg")), QString(), m_helpTextBrowser, [this]() {
        if (m_homeSource.isEmpty())
            m_helpTextBrowser->home();
        else
            m_helpTextBrowser->setSource(m_homeSource);
    });

    m_backContentAction = toolBar->addAction(QIcon(QStringLiteral(":/images/icon-arrow-left.svg")), QString(),
                                             m_helpTextBrowser, &HelpTextBrowser::backward);

    m_forwardContentAction = toolBar->addAction(QIcon(QStringLiteral(":/images/icon-arrow-right.svg")), QString(),
                                                m_helpTextBrowser, &HelpTextBrowser::forward);

    connect(m_helpTextBrowser, &HelpTextBrowser::backwardAvailable, m_backContentAction, &QAction::setEnabled);
    connect(m_helpTextBrowser, &HelpTextBrowser::forwardAvailable, m_forwardContentAction, &QAction::setEnabled);

    m_showHideContentsAction->setShortcut(Qt::Key_F2);
    m_backContentAction->setShortcut(QKeySequence::MoveToPreviousPage);
    m_homeContentAction->setShortcut(QKeySequence::MoveToStartOfLine);
    m_forwardContentAction->setShortcut(QKeySequence::MoveToNextPage);

    addToolBar(toolBar);
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
