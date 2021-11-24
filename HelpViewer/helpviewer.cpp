#include "helpviewer.h"

#include <QDesktopServices>



HelpViewer::HelpViewer(QWidget *parent) :
    QObject(parent),
    m_basisWidget(parent)
{
}



HelpViewer::HelpViewer(const QString &collectionFile, QWidget *parent) :
    QObject(parent),
    m_collectionFile(collectionFile),
    m_basisWidget(parent)
{
}



HelpViewer::~HelpViewer()
{
    close();
}



void HelpViewer::setHomeSource(const QUrl &url)
{
    m_homeUrl = url;

    if (m_helpWindow != nullptr && !m_helpEngine->isEmpty(url))
        m_helpWindow->setHomeSource(url);
}



void HelpViewer::setOpenExternalLinksEnabled(const bool enabled)
{
    m_openExternalLinksEnabled = enabled;

    if (m_helpWindow != nullptr)
        m_helpWindow->setOpenExternalLinksEnabled(enabled);
}



void HelpViewer::setWindowState(const Qt::WindowStates states)
{
    m_windowState = states;

    if (m_helpWindow != nullptr)
        m_helpWindow->setWindowState(states);
}



void HelpViewer::setWindowTitle(const QString &title)
{
    m_windowTitle = title;

    if (m_helpWindow != nullptr)
        m_helpWindow->setWindowTitle(title);
}



void HelpViewer::setWindowPosition(const QPoint &position)
{
    m_windowPosition = position;
    m_isWindowPositionDefined = true;

    if (m_helpWindow != nullptr)
        m_helpWindow->move(position);
}



void HelpViewer::setWindowSize(const QSize &size)
{
    if (size.isNull())
        return;

    m_windowSize = size;

    if (m_helpWindow != nullptr)
        m_helpWindow->setSize(m_windowSize, m_isWindowPositionDefined ? nullptr : m_basisWidget);
}



void HelpViewer::setWindowSplitterSizes(const QList<int> &sizes)
{
    if (sizes.count() != 2)
        return;

    m_windowSplitterSizes = sizes;

    if (m_helpWindow != nullptr)
        m_helpWindow->setSplitterSizes(sizes);
}



bool HelpViewer::open(const QUrl &url)
{
    if (!HelpTextBrowser::isUrlHttp(url)) {
        if (check(url)) {
            _open(url);
            return true;
        }
    }

    else if (m_openExternalLinksEnabled)
        return QDesktopServices::openUrl(url);

    return false;
}



bool HelpViewer::check(const QUrl &url)
{
    if (m_collectionFile.isEmpty())
        return false;

    QSharedPointer<HelpEngine> helpEngine;

    if (!m_helpEngine.isNull() && m_helpEngine->collectionFile() == m_collectionFile)
        helpEngine = m_helpEngine;

    else {
        helpEngine = QSharedPointer<HelpEngine>::create(m_collectionFile);
        if (!helpEngine->setupData())
            return false;
    }

    bool ok = false;

    if (!url.isEmpty()) {
        if (!helpEngine->isEmpty(url)) {
            m_lastValidUrl = url;
            ok = true;
        }
    }

    else if (!m_lastValidUrl.isEmpty() && !helpEngine->isEmpty(m_lastValidUrl))
        ok = true;

    else if (!m_homeUrl.isEmpty() && !helpEngine->isEmpty(m_homeUrl)) {
        m_lastValidUrl = m_homeUrl;
        ok = true;
    }

    else {
        const QUrl fileUrl = helpEngine->fileUrl(QStringLiteral("index"));
        if (!fileUrl.isEmpty()) {
            m_lastValidUrl = fileUrl;
            ok = true;
        }
    }

    if (ok) {
        m_helpEngine = helpEngine;
        m_helpEngine->setCollectionFile(m_collectionFile);
        return true;
    }

    return false;
}



void HelpViewer::_open(const QUrl &url)
{
    if (m_helpWindow == nullptr) {
        m_helpWindow = new HelpWindow(m_helpEngine.data());

        m_helpWindow->setSource(m_lastValidUrl);

        if (!m_helpEngine->isEmpty(m_homeUrl))
            m_helpWindow->setHomeSource(m_homeUrl);

        m_helpWindow->setOpenExternalLinksEnabled(m_openExternalLinksEnabled);

        m_helpWindow->setWindowState(m_windowState);
        m_helpWindow->setWindowTitle(m_windowTitle);

        if (m_isWindowPositionDefined) {
            m_helpWindow->move(m_windowPosition);
            m_helpWindow->setSize(m_windowSize);
        }
        else
            m_helpWindow->setSize(m_windowSize, m_basisWidget);

        m_helpWindow->setSplitterSizes(m_windowSplitterSizes);

        connect(m_helpWindow, &HelpWindow::destroyed, this, &HelpViewer::onWindowDestroyed);

        m_helpWindow->show();
    }

    else {
        m_helpWindow->setSource(url);
        m_helpWindow->activateWindow();
    }
}



void HelpViewer::onWindowDestroyed()
{
    m_lastValidUrl = m_helpWindow->lastSource();
    m_windowState = m_helpWindow->windowState();
    m_windowPosition = m_helpWindow->pos();
    m_isWindowPositionDefined = true;
    m_windowSize = m_helpWindow->size();
    m_windowSplitterSizes = m_helpWindow->splitterSizes();
    m_helpWindow = nullptr;
    m_helpEngine = QSharedPointer<HelpEngine>(nullptr);
}



void HelpViewer::close()
{
    if (m_helpWindow != nullptr)
        m_helpWindow->close();
}
