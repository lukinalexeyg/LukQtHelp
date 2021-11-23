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

    void setSource(const QUrl &source);
    void setHomeSource(const QUrl &source);
    QUrl lastSource() const;

    inline void setOpenExternalLinksEnabled(const bool enabled) { m_helpTextBrowser->setOpenExternalLinksEnabled(enabled); }
    inline bool openExternalLinksEnabled() const                { return m_helpTextBrowser->openExternalLinksEnabled(); }

    inline void setSplitterSizes(const QList<int> &sizes)       { m_splitter->setSizes(sizes); }
    inline QList<int> splitterSizes() const                     { return m_splitter->sizes(); }

private:
    QHelpEngine *m_helpEngine;
    HelpTextBrowser *m_helpTextBrowser;
    QSplitter *m_splitter;
    QList<int> m_splitterSizes;
    QStatusBar *m_statusBar;
    QUrl m_homeSource;

private:
    void setToolBar();
    void setWidgets();
    void _setStatusBar();
    void showHideContents();
};

#endif // HELPWINDOW_H
