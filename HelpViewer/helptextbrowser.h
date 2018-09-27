#ifndef HELPTEXTBROWSER_H
#define HELPTEXTBROWSER_H

#pragma once

#include <QTextBrowser>
#include <QtHelp/QHelpEngine>
#include <QWheelEvent>

class HelpTextBrowser : public QTextBrowser
{
public:
    HelpTextBrowser(QHelpEngine *helpEngine, QWidget *parent = nullptr);
    void setSource(const QUrl &url);
    inline void _setOpenExternalLinks(bool open) {m_openExternalLinks = open;}

protected:
    void wheelEvent(QWheelEvent *event);

private:
    QHelpEngine *helpEngine;
    bool m_openExternalLinks;
    QVariant loadResource (int type, const QUrl &name);
};

#endif // HELPTEXTBROWSER_H
