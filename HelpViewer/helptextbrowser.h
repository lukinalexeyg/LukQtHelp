#ifndef HELPTEXTBROWSER_H
#define HELPTEXTBROWSER_H

#include <QTextBrowser>
#include <QtHelp/QHelpEngine>
#include <QWheelEvent>

class HelpTextBrowser : public QTextBrowser
{
public:
    HelpTextBrowser(QHelpEngine *helpEngine, QWidget *parent = nullptr);

    virtual void setSource(const QUrl &url) override;

    inline void setOpenExternalLinksEnabled(const bool enabled) { m_openExternalLinksEnabled = enabled; }
    inline bool openExternalLinksEnabled() const                { return m_openExternalLinksEnabled; }

    static bool isUrlHttp(const QUrl &url);
    static bool isUrlHelp(const QUrl &url);

protected:
    virtual void wheelEvent(QWheelEvent *event) override;

private:
    QHelpEngine *m_helpEngine;
    bool m_openExternalLinksEnabled;

private:
    virtual QVariant loadResource (const int type, const QUrl &name) override;
};

#endif // HELPTEXTBROWSER_H
