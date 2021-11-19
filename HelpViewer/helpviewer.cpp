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
    m_basisWidget(parent),
    m_collectionFile(collectionFile)
{
}



HelpViewer::~HelpViewer()
{
    close();
}



void HelpViewer::setWindowTitle(const QString &title)
{
    m_helpWindowTitle = title;
    if (m_helpWindow != nullptr)
        m_helpWindow->setWindowTitle(m_helpWindowTitle);
}



void HelpViewer::setCollectionFile(const QString &collectionFile)
{
    if (collectionFile != m_collectionFile) {
        m_collectionFile = collectionFile;
        m_collectionFileChanged = true;
    }
}



void HelpViewer::setHomeSource(const QString &source)
{
    m_homeSource = source;
    if (m_helpWindow != nullptr)
        m_helpWindow->setHomeSource(m_homeSource);
}



void HelpViewer::setOpenExternalLinksEnabled(const bool enabled)
{
    m_openExternalLinks = enabled;
    if (m_helpWindow != nullptr)
        m_helpWindow->setOpenExternalLinksEnabled(m_openExternalLinks);
}



bool HelpViewer::show(const QString &source)
{
    if (!source.startsWith(QStringLiteral("http"))) {
        if (check(source)) {
            m_source = source;
            moveToThread(QApplication::instance()->thread());
            QCoreApplication::postEvent(this, new QEvent(QEvent::User));
            return true;
        }
    }
    else if (m_openExternalLinks)
        return QDesktopServices::openUrl(source);

    return false;
}



bool HelpViewer::event(QEvent *event)
{
    if (event->type() == QEvent::User) {

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

            m_helpWindow->setHorizontalSplitterSizes(m_horizontalSplitterSizes);
            m_helpWindow->setSource(m_lastValidSource);
            m_helpWindow->setHomeSource(m_homeSource);
            m_helpWindow->setOpenExternalLinksEnabled(m_openExternalLinks);

            connect(m_helpWindow, &HelpWindow::destroyed, this, [this] {
                m_helpWindowStates = m_helpWindow->windowState();
                m_helpWindowPosition = m_helpWindow->pos();
                m_helpWindowSize = m_helpWindow->size();
                m_horizontalSplitterSizes = m_helpWindow->horizontalSplitterSizes();
                m_lastValidSource = m_helpWindow->lastSource().toString();
                m_helpWindow = nullptr;
                m_helpEngine->deleteLater();
                m_helpEngine = nullptr;
            });

            m_helpWindow->show();
        }

        else {
            m_helpWindow->setSource(m_source);
            m_helpWindow->activateWindow();
        }

        return true;
    }

    return QObject::event(event);
}



bool HelpViewer::check(const QString &source)
{
    if (m_collectionFile.isEmpty())
        return false;

    QHelpEngine *checkHelpEngine;
    bool sucessful = true;

    if (m_helpEngine != nullptr && !m_collectionFileChanged)
        checkHelpEngine = m_helpEngine;
    else {
        checkHelpEngine = new QHelpEngine(m_collectionFile, this);
        if (!checkHelpEngine->setupData())
            sucessful = false;
    }

    if (sucessful) {
        if (!source.isEmpty()) {
            if (!checkHelpEngine->fileData(QUrl(source)).isEmpty())
                m_lastValidSource = source;
            else
                sucessful = false;
        }

        else if (!m_lastValidSource.isEmpty() && !checkHelpEngine->fileData(m_lastValidSource).isEmpty()) {}

        else if (!m_homeSource.isEmpty() && !checkHelpEngine->fileData(m_homeSource).isEmpty())
            m_lastValidSource = m_homeSource;

        else {
            const QStringList startFiles = {QStringLiteral("index.html"), QStringLiteral("index.htm")};
            sucessful = false;

            for (const QString &registeredDocumentation : checkHelpEngine->registeredDocumentations()) {

                for (const QUrl &file : checkHelpEngine->files(registeredDocumentation, QStringList())) {
                    const QString source = file.toString();
                    const QString fileName = source.section(QStringLiteral("/"), -1);

                    for (const QString &startFile : qAsConst(startFiles)) {
                        if (startFile == fileName && !checkHelpEngine->fileData(file).isEmpty()) {
                            m_lastValidSource = source;
                            sucessful = true;
                            goto DONE;
                        }
                    }

                    if (!checkHelpEngine->fileData(file).isEmpty()) {
                        m_lastValidSource = source;
                        sucessful = true;
                        goto DONE;
                    }
                }
            }
        }
    }

    DONE:

    if (m_helpEngine == nullptr) {
        if (sucessful)
            m_helpEngine = checkHelpEngine;
        else
            checkHelpEngine->deleteLater();
    }
    else if (m_collectionFileChanged) {
        if (sucessful)
            m_helpEngine->setCollectionFile(m_collectionFile);
        checkHelpEngine->deleteLater();
    }

    return sucessful;
}



void HelpViewer::close()
{
    if (m_helpWindow != nullptr)
        m_helpWindow->close();
}
