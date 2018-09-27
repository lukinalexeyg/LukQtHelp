#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#pragma once

#include <QSplitter>
#include "independentwindow.h"
#include "helptextbrowser.h"

class HelpWindow : public IndependentWindow
{
    Q_OBJECT

public:
    HelpWindow(QHelpEngine *helpEngine, QWidget *parent = nullptr);

    void setSource(const QString &source);
    void setHomeSource(const QString &source);
    QString lastSource();

    inline void setOpenExternalLinks(bool open)                     {helpTextBrowser->_setOpenExternalLinks(open);}
    inline bool openExternalLinks()                                 {return helpTextBrowser->openExternalLinks();}

    inline void setHorizontalSplitterSizes(const QList<int> &sizes) {horizontalSplitter->setSizes(sizes);}
    inline QList<int> horizontalSplitterSizes()                     {return horizontalSplitter->sizes();}

protected:
    void changeEvent(QEvent *event);

private:
    QHelpEngine *helpEngine;
    HelpTextBrowser *helpTextBrowser;
    QHelpContentModel *helpContentModel;
    QSplitter *horizontalSplitter;
    QList<int> m_horizontalSplitterSizes = {200, 600};
    QStatusBar *statusBar;
    QUrl homeSource;

    QAction *showHideContentsAction;
    QAction *homeContentAction;
    QAction *backContentAction;
    QAction *forwardContentAction;

    void retranslate();
    void setToolBar();
    void setContent();
    void showHideContents();
};

#endif // HELPWINDOW_H
