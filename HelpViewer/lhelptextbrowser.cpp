#include "lhelptextbrowser.h"

#include <QDesktopServices>



LHelpTextBrowser::LHelpTextBrowser(QHelpEngine *helpEngine, QWidget *parent) :
    QTextBrowser(parent),
    m_helpEngine(helpEngine)
{
}



void LHelpTextBrowser::setSource(const QUrl &url)
{
    if (!isUrlHttp(url))
        QTextBrowser::setSource(url);

    else if (m_externalLinksEnabled)
        QDesktopServices::openUrl(url);
}



QVariant LHelpTextBrowser::loadResource(const int type, const QUrl &name)
{
    if (isUrlHelp(name))
        return m_helpEngine->fileData(name);

    return QTextBrowser::loadResource(type, name);
}



void LHelpTextBrowser::wheelEvent(QWheelEvent *event)
{
    if (!event->modifiers().testFlag(Qt::ControlModifier))
        QTextBrowser::wheelEvent(event);
}



bool LHelpTextBrowser::isUrlHttp(const QUrl &url)
{
    return url.scheme().startsWith(QLatin1String("http"));
}



bool LHelpTextBrowser::isUrlHelp(const QUrl &url)
{
    return url.scheme() == QLatin1String("qthelp");
}
