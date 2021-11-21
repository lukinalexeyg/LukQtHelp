#include "helpviewer.h"

#include <QApplication>
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



void HelpViewer::setCollectionFile(const QString &collectionFile)
{
    if (collectionFile != m_collectionFile) {
        m_collectionFile = collectionFile;
        m_collectionFileChanged = true;
    }
}



void HelpViewer::setWindowTitle(const QString &title)
{
    m_helpWindowTitle = title;

    if (m_helpWindow != nullptr)
        m_helpWindow->setWindowTitle(title);
}



void HelpViewer::setHomeSource(const QUrl &source)
{
    m_homeSource = source;

    if (m_helpWindow != nullptr)
        m_helpWindow->setHomeSource(source);
}



void HelpViewer::setOpenExternalLinksEnabled(const bool enabled)
{
    m_openExternalLinksEnabled = enabled;

    if (m_helpWindow != nullptr)
        m_helpWindow->setOpenExternalLinksEnabled(enabled);
}



bool HelpViewer::open(const QUrl &source)
{
    if (!HelpTextBrowser::isUrlHttp(source)) {
        if (check(source)) {
            _open(source);
            return true;
        }
    }

    else if (m_openExternalLinksEnabled)
        return QDesktopServices::openUrl(source);

    return false;
}



bool HelpViewer::check(const QUrl &source)
{
    if (m_collectionFile.isEmpty())
        return false;

    QHelpEngine *helpEngine;
    bool ok = true;

    if (m_helpEngine != nullptr && !m_collectionFileChanged)
        helpEngine = m_helpEngine;
    else {
        helpEngine = new QHelpEngine(m_collectionFile, this);
        if (!helpEngine->setupData())
            ok = false;
    }

    if (ok) {
        if (!source.isEmpty()) {
            if (!helpEngine->fileData(source).isEmpty())
                m_lastValidSource = source;
            else
                ok = false;
        }

        else if (!m_lastValidSource.isEmpty() && !helpEngine->fileData(m_lastValidSource).isEmpty()) {
            // use m_lastValidSource
        }

        else if (!m_homeSource.isEmpty() && !helpEngine->fileData(m_homeSource).isEmpty()) {
            // use m_homeSource
            m_lastValidSource = m_homeSource;
        }

        else {
            const QUrl file = findFile(helpEngine, QLatin1String("index"));

            if (file.isEmpty())
                ok = false;
            else {
                m_lastValidSource = file;
                ok = true;
            }
        }
    }

    if (m_helpEngine == nullptr) {
        if (ok)
            m_helpEngine = helpEngine;
        else
            helpEngine->deleteLater();
    }
    else if (m_collectionFileChanged) {
        if (ok)
            m_helpEngine->setCollectionFile(m_collectionFile);
        helpEngine->deleteLater();
    }

    return ok;
}



QUrl HelpViewer::findFile(QHelpEngine *helpEngine, const QString &searchFileName) const
{
    const QStringList documentations = helpEngine->registeredDocumentations();
    QUrl firstFile;

    for (const QString &documentation : qAsConst(documentations)) {
        const QList<QUrl> files = helpEngine->files(documentation, QString());

        for (const QUrl &file : qAsConst(files)) {
            const QString fileName = file.fileName();

            if (!fileName.section(QLatin1Char('.'), -1).startsWith(QLatin1String("htm")))
                continue;

            if (firstFile.isEmpty() && !helpEngine->fileData(file).isEmpty()) {
                if (fileName.startsWith(searchFileName))
                    return file;
                firstFile = file;
            }

            if (fileName.startsWith(searchFileName)) {
                if (!helpEngine->fileData(file).isEmpty())
                    return file;
                if (!firstFile.isEmpty())
                    return firstFile;
            }
        }
    }

    return firstFile;
}



void HelpViewer::_open(const QUrl &source)
{
    if (m_helpWindow == nullptr) {
        m_helpWindow = new HelpWindow(m_helpEngine);
        m_helpWindow->setWindowState(m_helpWindowStates);
        m_helpWindow->setWindowTitle(m_helpWindowTitle);

        if (!m_helpWindowPosition.isNull()) {
            m_helpWindow->move(m_helpWindowPosition);
            m_helpWindow->setSize(m_helpWindowSize);
        }
        else
            m_helpWindow->setSize(m_helpWindowSize, m_basisWidget);

        m_helpWindow->setHorizontalSplitterSizes(m_helpWindowHorizontalSplitterSizes);
        m_helpWindow->setSource(m_lastValidSource);
        m_helpWindow->setHomeSource(m_homeSource);
        m_helpWindow->setOpenExternalLinksEnabled(m_openExternalLinksEnabled);

        connect(m_helpWindow, &HelpWindow::destroyed, this, [this] {
            m_helpWindowStates = m_helpWindow->windowState();
            m_helpWindowPosition = m_helpWindow->pos();
            m_helpWindowSize = m_helpWindow->size();
            m_helpWindowHorizontalSplitterSizes = m_helpWindow->horizontalSplitterSizes();
            m_lastValidSource = m_helpWindow->lastSource().toString();
            m_helpWindow = nullptr;
            m_helpEngine->deleteLater();
            m_helpEngine = nullptr;
        });

        m_helpWindow->show();
    }

    else {
        m_helpWindow->setSource(source);
        m_helpWindow->activateWindow();
    }
}



void HelpViewer::close()
{
    if (m_helpWindow != nullptr)
        m_helpWindow->close();
}
