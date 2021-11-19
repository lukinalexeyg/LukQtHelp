#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include "independentwindow.h"
#include "helptextbrowser.h"

#include <QSplitter>

class HelpWindow : public IndependentWindow
{
    Q_OBJECT

public:
    HelpWindow(QHelpEngine *helpEngine, QWidget *parent = nullptr);

    void setSource(const QString &source);
    void setHomeSource(const QString &source);
    QString lastSource() const;

    inline void setOpenExternalLinksEnabled(const bool enabled)     { m_helpTextBrowser->setOpenExternalLinksEnabled(enabled); }
    inline bool openExternalLinks() const                           { return m_helpTextBrowser->openExternalLinks(); }

    inline void setHorizontalSplitterSizes(const QList<int> &sizes) { m_horizontalSplitter->setSizes(sizes); }
    inline QList<int> horizontalSplitterSizes() const               { return m_horizontalSplitter->sizes(); }

protected:
    void changeEvent(QEvent *event);

private:
    QHelpEngine *m_helpEngine;
    HelpTextBrowser *m_helpTextBrowser;
    QHelpContentModel *m_helpContentModel;
    QSplitter *m_horizontalSplitter;
    QList<int> m_horizontalSplitterSizes = { 200, 600 };
    QStatusBar *m_statusBar;
    QUrl m_homeSource;

    QAction *m_showHideContentsAction;
    QAction *m_homeContentAction;
    QAction *m_backContentAction;
    QAction *m_forwardContentAction;

private:
    void retranslate();
    void setToolBar();
    void setContent();
    void showHideContents();
};

#endif // HELPWINDOW_H
