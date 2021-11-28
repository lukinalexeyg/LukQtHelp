#include "lhelpviewer.h"

#include <QDesktopServices>



LHelpViewer::LHelpViewer(QWidget *parent) :
    QObject(parent),
    m_basisWidget(parent)
{
}



LHelpViewer::LHelpViewer(const QString &collectionFile, QWidget *parent) :
    QObject(parent),
    m_collectionFile(collectionFile),
    m_basisWidget(parent)
{
}



LHelpViewer::~LHelpViewer()
{
    close();
}



void LHelpViewer::setHomeSource(const QUrl &url)
{
    m_homeUrl = url;

    if (m_helpWindow != nullptr && !m_helpEngine->isEmpty(url))
        m_helpWindow->setHomeSource(url);
}



void LHelpViewer::setExternalLinksEnabled(const bool enabled)
{
    m_externalLinksEnabled = enabled;

    if (m_helpWindow != nullptr)
        m_helpWindow->setExternalLinksEnabled(enabled);
}



void LHelpViewer::setWindowState(const Qt::WindowStates states)
{
    m_windowState = states;

    if (m_helpWindow != nullptr)
        m_helpWindow->setWindowState(states);
}



void LHelpViewer::setWindowTitle(const QString &title)
{
    m_windowTitle = title;

    if (m_helpWindow != nullptr)
        m_helpWindow->setWindowTitle(title);
}



void LHelpViewer::setWindowPosition(const QPoint &position)
{
    m_windowPosition = position;
    m_isWindowPositionDefined = true;

    if (m_helpWindow != nullptr)
        m_helpWindow->move(position);
}



void LHelpViewer::setWindowSize(const QSize &size)
{
    if (size.isNull())
        return;

    m_windowSize = size;

    if (m_helpWindow != nullptr)
        m_helpWindow->setSize(m_windowSize, m_isWindowPositionDefined ? nullptr : m_basisWidget);
}



void LHelpViewer::setWindowSplitterSizes(const QList<int> &sizes)
{
    if (sizes.count() != 2)
        return;

    m_windowSplitterSizes = sizes;

    if (m_helpWindow != nullptr)
        m_helpWindow->setSplitterSizes(sizes);
}



bool LHelpViewer::open(const QUrl &url)
{
    m_lastError = NoError;

    if (url.isEmpty() || LHelpTextBrowser::isUrlHelp(url)) {
        if (check(url)) {
            _open(url);
            return true;
        }
    }

    else if (LHelpTextBrowser::isUrlHttp(url)) {
        if (m_externalLinksEnabled) {
            if (QDesktopServices::openUrl(url))
                return true;
            m_lastError = ExternalLinkOpenError;
        }
        else
            m_lastError = ExternalLinksAreDisabled;
    }

    else
        m_lastError = InvalidSheme;

    return false;
}



bool LHelpViewer::check(const QUrl &url)
{
    if (m_collectionFile.isEmpty()) {
        m_lastError = CollectionFileIsUndefined;
        return false;
    }

    if (!QFile::exists(m_collectionFile)) {
        m_lastError = CollectionFileNotFound;
        return false;
    }

    QSharedPointer<LHelpEngine> helpEngine;

    if (!m_helpEngine.isNull() && m_helpEngine->collectionFile() == m_collectionFile)
        helpEngine = m_helpEngine;

    else {
        helpEngine = QSharedPointer<LHelpEngine>::create(m_collectionFile);
        if (!helpEngine->setupData()) {
            m_lastError = CollectionFileSetupError;
            return false;
        }
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

    m_lastError = DataFileIsEmpty;
    return false;
}



void LHelpViewer::_open(const QUrl &url)
{
    if (m_helpWindow == nullptr) {
        m_helpWindow = new LHelpWindow(m_helpEngine.data());

        m_helpWindow->setSource(m_lastValidUrl);

        if (!m_helpEngine->isEmpty(m_homeUrl))
            m_helpWindow->setHomeSource(m_homeUrl);

        m_helpWindow->setExternalLinksEnabled(m_externalLinksEnabled);

        m_helpWindow->setWindowState(m_windowState);
        m_helpWindow->setWindowTitle(m_windowTitle);

        if (m_isWindowPositionDefined) {
            m_helpWindow->move(m_windowPosition);
            m_helpWindow->setSize(m_windowSize);
        }
        else
            m_helpWindow->setSize(m_windowSize, m_basisWidget);

        m_helpWindow->setSplitterSizes(m_windowSplitterSizes);

        connect(m_helpWindow, &LHelpWindow::destroyed, this, &LHelpViewer::onWindowDestroyed);

        m_helpWindow->show();
    }

    else {
        m_helpWindow->setSource(url);
        m_helpWindow->activateWindow();
    }
}



void LHelpViewer::onWindowDestroyed()
{
    m_lastValidUrl = m_helpWindow->lastSource();
    m_windowState = m_helpWindow->windowState();
    m_windowPosition = m_helpWindow->pos();
    m_isWindowPositionDefined = true;
    m_windowSize = m_helpWindow->size();
    m_windowSplitterSizes = m_helpWindow->splitterSizes();
    m_helpWindow = nullptr;
    m_helpEngine = QSharedPointer<LHelpEngine>(nullptr);
}



void LHelpViewer::close()
{
    if (m_helpWindow != nullptr)
        m_helpWindow->close();
}
