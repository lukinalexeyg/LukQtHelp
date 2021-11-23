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
    void setHomeSource(const QUrl &source) { m_homeSource = source; }
    QUrl lastSource() const;

    void setOpenExternalLinksEnabled(const bool enabled);
    bool openExternalLinksEnabled() const;

    void setSplitterSizes(const QList<int> &sizes);
    QList<int> splitterSizes() const;

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

    void onContentWidgetPressed(const QModelIndex &modelIndex);
    void onToolBarHomeTriggered();
    void onTextBrowserHighlighted(const QUrl &link);
};

#endif // HELPWINDOW_H
