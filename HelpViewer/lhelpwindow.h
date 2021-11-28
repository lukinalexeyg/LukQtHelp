#ifndef LHELPWINDOW_H
#define LHELPWINDOW_H

#include "lstandalonemainwindow.h"
#include "lhelptextbrowser.h"

#include <QSplitter>

class LHelpWindow : public LStandaloneMainWindow
{
    Q_OBJECT

public:
    LHelpWindow(QHelpEngine *helpEngine, QWidget *parent = nullptr);

    void setSource(const QUrl &source);
    void setHomeSource(const QUrl &source) { m_homeSource = source; }
    QUrl lastSource() const;

    void setExternalLinksEnabled(const bool enabled);
    bool externalLinksEnabled() const;

    void setSplitterSizes(const QList<int> &sizes);
    QList<int> splitterSizes() const;

private:
    QHelpEngine *m_helpEngine;
    LHelpTextBrowser *m_helpTextBrowser;
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

#endif // LHELPWINDOW_H
