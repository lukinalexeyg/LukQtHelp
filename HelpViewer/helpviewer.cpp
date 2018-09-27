#include <QApplication>
#include <QDesktopServices>
#include "helpviewer.h"



HelpViewer::HelpViewer(QWidget *parent) :
    QObject(parent),
    m_basisWidget(parent)
{}



HelpViewer::HelpViewer(const QString &collectionFile, QWidget *parent) :
    QObject(parent),
    m_basisWidget(parent),
    m_collectionFile(collectionFile)
{}



HelpViewer::~HelpViewer()
{
    close();
}



void HelpViewer::setWindowTitle(const QString &title)
{
    helpWindowTitle = title;
    if (helpWindow != Q_NULLPTR)
        helpWindow->setWindowTitle(helpWindowTitle);
}



void HelpViewer::setCollectionFile(const QString &collectionFile)
{
    if (collectionFile != m_collectionFile) {
        m_collectionFile = collectionFile;
        collectionFileChanged = true;
    }
}



void HelpViewer::setHomeSource(const QString &source)
{
    m_homeSource = source;
    if (helpWindow != Q_NULLPTR)
        helpWindow->setHomeSource(m_homeSource);
}



void HelpViewer::setOpenExternalLinks(bool open)
{
    m_openExternalLinks = open;
    if (helpWindow != Q_NULLPTR)
        helpWindow->setOpenExternalLinks(m_openExternalLinks);
}



bool HelpViewer::show(const QString &source)
{
    if (!source.startsWith("http")) {
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

        if (helpWindow == Q_NULLPTR) {
            helpWindow = new HelpWindow(helpEngine);
            helpWindow->setWindowState(helpWindowStates);
            helpWindow->setWindowTitle(helpWindowTitle);

            if (!helpWindowPosition.isNull()) {
                helpWindow->move(helpWindowPosition);
                helpWindow->setSize(helpWindowSize);
            }
            else
                helpWindow->setSize(helpWindowSize, m_basisWidget);

            helpWindow->setHorizontalSplitterSizes(horizontalSplitterSizes);
            helpWindow->setSource(m_lastValidSource);
            helpWindow->setHomeSource(m_homeSource);
            helpWindow->setOpenExternalLinks(m_openExternalLinks);

            connect(helpWindow, &HelpWindow::destroyed, [this](){
                helpWindowStates = helpWindow->windowState();
                helpWindowPosition = helpWindow->pos();
                helpWindowSize = helpWindow->size();
                horizontalSplitterSizes = helpWindow->horizontalSplitterSizes();
                m_lastValidSource = helpWindow->lastSource();
                helpWindow = Q_NULLPTR;
                helpEngine->deleteLater();
                helpEngine = Q_NULLPTR;
            });

            helpWindow->show();
        }

        else {
            helpWindow->setSource(m_source);
            helpWindow->activateWindow();
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

    if (helpEngine != Q_NULLPTR && !collectionFileChanged)
        checkHelpEngine = helpEngine;
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
            QStringList startFiles = {"index.html", "index.htm"};
            sucessful = false;

            for (const QString &registeredDocumentation : checkHelpEngine->registeredDocumentations()) {

                for (const QUrl &file : checkHelpEngine->files(registeredDocumentation, QStringList())) {
                    const QString source = file.toString();
                    const QString fileName = source.section('/', -1);

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

    if (helpEngine == Q_NULLPTR) {
        if (sucessful)
            helpEngine = checkHelpEngine;
        else
            checkHelpEngine->deleteLater();
    }
    else if (collectionFileChanged) {
        if (sucessful)
            helpEngine->setCollectionFile(m_collectionFile);
        checkHelpEngine->deleteLater();
    }

    return sucessful;
}



void HelpViewer::close()
{
    if (helpWindow != Q_NULLPTR)
        helpWindow->close();
}
