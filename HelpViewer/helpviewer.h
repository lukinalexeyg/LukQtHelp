#ifndef HELPVIEWER_H
#define HELPVIEWER_H

#pragma once

#include "helpwindow.h"

class HelpViewer : public QObject
{
    Q_OBJECT

public:
    explicit HelpViewer(QWidget *parent = nullptr);
    explicit HelpViewer(const QString &collectionFile, QWidget *parent = nullptr);
    ~HelpViewer();

    inline void setBasisWidget(QWidget *widget = nullptr)   {m_basisWidget = widget;}
    inline QWidget *basisWidget()                           {return m_basisWidget;}

    void setWindowTitle(const QString &title);
    inline QString windowTitle()                            {return helpWindowTitle;}

    void setCollectionFile(const QString &collectionFile);
    inline QString collectionFile()                         {return m_collectionFile;}

    void setHomeSource(const QString &source);
    inline QString homeSource()                             {return m_homeSource;}

    void setOpenExternalLinks(bool open);
    inline bool openExternalLinks()                         {return m_openExternalLinks;}

    bool show(const QString &source = "");
    void close();

protected:
    virtual bool event(QEvent *event);

private:
    QWidget *m_basisWidget;

    QHelpEngine *helpEngine = Q_NULLPTR;
    HelpWindow *helpWindow = Q_NULLPTR;

    Qt::WindowStates helpWindowStates;
    QString helpWindowTitle;
    QPoint helpWindowPosition;
    QSize helpWindowSize = QSize(960, 600);
    QList<int> horizontalSplitterSizes = {200, 600};

    QString m_collectionFile = "";
    bool collectionFileChanged = false;

    QString m_lastValidSource = "";
    QString m_homeSource = "";
    QString m_source;

    bool m_openExternalLinks = true;

    bool check(const QString &source);
};

#endif // HELPMANAGER_H
