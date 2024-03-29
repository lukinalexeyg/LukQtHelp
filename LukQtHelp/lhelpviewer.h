#ifndef LHELPVIEWER_H
#define LHELPVIEWER_H

#include "lhelpengine.h"
#include "lhelpwindow.h"

class LHelpViewer : public QObject
{
    Q_OBJECT

public:
    enum Error {
        NoError,
        CollectionFileIsUndefined,
        CollectionFileNotFound,
        CollectionFileSetupError,
        DataFileIsEmpty,
        ExternalLinksAreDisabled,
        ExternalLinkOpenError,
        InvalidSheme
    };

public:
    explicit LHelpViewer(QWidget *parent = nullptr);
    explicit LHelpViewer(const QString &collectionFile, QWidget *parent = nullptr);

    ~LHelpViewer();

    void setCollectionFile(const QString &collectionFile)   { m_collectionFile = collectionFile; }
    QString collectionFile() const                          { return m_collectionFile; }

    void setHomeSource(const QUrl &url);
    QUrl homeSource() const                                 { return m_homeUrl; }

    void setExternalLinksEnabled(const bool enabled);
    bool externalLinksEnabled() const                       { return m_externalLinksEnabled; }

    void setBasisWidget(QWidget *widget = nullptr)          { m_basisWidget = widget; }
    QWidget *basisWidget() const                            { return m_basisWidget; }

    void setWindowState(const Qt::WindowStates states);
    Qt::WindowStates windowState() const                    { return m_windowState; }

    void setWindowTitle(const QString &title);
    QString windowTitle() const                             { return m_windowTitle; }

    void setWindowPosition(const QPoint &position);
    QPoint windowPosition() const                           { return m_windowPosition; }

    void setWindowSize(const QSize &size);
    QSize windowSize() const                                { return m_windowSize; }

    void setWindowSplitterSizes(const QList<int> &sizes);
    QList<int> windowSplitterSizes() const                  { return m_windowSplitterSizes; }

    bool open(const QUrl &url = QUrl());
    void close();

    Error lastError() const                                 { return m_lastError; }

private:
    QSharedPointer<LHelpEngine> m_helpEngine;
    QString m_collectionFile;
    QUrl m_homeUrl;
    QUrl m_lastValidUrl;
    bool m_externalLinksEnabled = true;

    QWidget *m_basisWidget;

    LHelpWindow *m_helpWindow = nullptr;
    Qt::WindowStates m_windowState;
    QString m_windowTitle;
    QPoint m_windowPosition;
    bool m_isWindowPositionDefined = false;
    QSize m_windowSize = QSize(960, 600);
    QList<int> m_windowSplitterSizes = { 200, 600 };

    Error m_lastError = NoError;

private:
    bool check(const QUrl &url);
    void _open(const QUrl &url);
    void onWindowDestroyed();
};

#endif // LHELPVIEWER_H
