#ifndef HELPVIEWER_H
#define HELPVIEWER_H

#include "helpengine.h"
#include "helpwindow.h"

class HelpViewer : public QObject
{
    Q_OBJECT

public:
    explicit HelpViewer(QWidget *parent = nullptr);
    explicit HelpViewer(const QString &collectionFile, QWidget *parent = nullptr);

    ~HelpViewer();

    inline void setBasisWidget(QWidget *widget = nullptr)           { m_basisWidget = widget; }
    inline QWidget *basisWidget() const                             { return m_basisWidget; }

    void setWindowTitle(const QString &title);
    inline QString windowTitle() const                              { return m_helpWindowTitle; }

    inline void setCollectionFile(const QString &collectionFile)    { m_collectionFile = collectionFile; }
    inline QString collectionFile() const                           { return m_collectionFile; }

    void setHomeSource(const QUrl &url);
    inline QUrl homeSource() const                                  { return m_homeUrl; }

    void setOpenExternalLinksEnabled(const bool enabled);
    inline bool openExternalLinksEnabled() const                    { return m_openExternalLinksEnabled; }

    bool open(const QUrl &url = QUrl());
    void close();

private:
    QSharedPointer<HelpEngine> m_helpEngine;
    QString m_collectionFile;

    QWidget *m_basisWidget;

    HelpWindow *m_helpWindow = nullptr;
    Qt::WindowStates m_helpWindowStates;
    QString m_helpWindowTitle;
    QPoint m_helpWindowPosition;
    QSize m_helpWindowSize = QSize(960, 600);
    QList<int> m_helpWindowHorizontalSplitterSizes = { 200, 600 };
    bool m_openExternalLinksEnabled = true;

    QUrl m_homeUrl;
    QUrl m_lastValidUrl;

private:
    bool check(const QUrl &url);
    void _open(const QUrl &url);
};

#endif // HELPVIEWER_H
