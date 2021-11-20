#include "helptextbrowser.h"

#include <QDesktopServices>



HelpTextBrowser::HelpTextBrowser(QHelpEngine *helpEngine, QWidget *parent) :
    QTextBrowser(parent),
    m_helpEngine(helpEngine)
{
}



void HelpTextBrowser::setSource(const QUrl &url)
{
    if (!isUrlHttp(url))
        QTextBrowser::setSource(url);

    else if (m_openExternalLinksEnabled)
        QDesktopServices::openUrl(url);
}



QVariant HelpTextBrowser::loadResource(const int type, const QUrl &name)
{
    if (isUrlHelp(name))
        return m_helpEngine->fileData(name);

    return QTextBrowser::loadResource(type, name);
}



void HelpTextBrowser::wheelEvent(QWheelEvent *event)
{
    if (!event->modifiers().testFlag(Qt::ControlModifier))
        QTextBrowser::wheelEvent(event);
}



bool HelpTextBrowser::isUrlHttp(const QUrl &url)
{
    return url.scheme().startsWith(QStringLiteral("http"));
}



bool HelpTextBrowser::isUrlHelp(const QUrl &url)
{
    return url.scheme() == QStringLiteral("qthelp");
}
