#ifndef HELPVIEWER_H
#define HELPVIEWER_H

#include "helpwindow.h"

class HelpViewer : public QObject
{
    Q_OBJECT

public:
    explicit HelpViewer(QWidget *parent = nullptr);
    explicit HelpViewer(const QString &collectionFile, QWidget *parent = nullptr);

    ~HelpViewer();

    inline void setBasisWidget(QWidget *widget = nullptr)   { m_basisWidget = widget; }
    inline QWidget *basisWidget() const                     { return m_basisWidget; }

    void setWindowTitle(const QString &title);
    inline QString windowTitle() const                      { return m_helpWindowTitle; }

    void setCollectionFile(const QString &collectionFile);
    inline QString collectionFile() const                   { return m_collectionFile; }

    void setHomeSource(const QString &source);
    inline QString homeSource() const                       { return m_homeSource; }

    void setOpenExternalLinksEnabled(const bool enabled);
    inline bool openExternalLinks() const                   { return m_openExternalLinks; }

    bool show(const QString &source = QString());
    void close();

protected:
    virtual bool event(QEvent *event) override;

private:
    QWidget *m_basisWidget;

    QHelpEngine *m_helpEngine = nullptr;
    HelpWindow *m_helpWindow = nullptr;

    Qt::WindowStates m_helpWindowStates;
    QString m_helpWindowTitle;
    QPoint m_helpWindowPosition;
    QSize m_helpWindowSize = QSize(960, 600);
    QList<int> m_horizontalSplitterSizes = { 200, 600 };

    QString m_collectionFile;
    bool m_collectionFileChanged = false;

    QString m_lastValidSource;
    QString m_homeSource;
    QString m_source;

    bool m_openExternalLinks = true;

private:
    bool check(const QString &source);
};

#endif // HELPVIEWER_H
