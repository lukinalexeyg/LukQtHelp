#include "helptextbrowser.h"

#include <QDesktopServices>



HelpTextBrowser::HelpTextBrowser(QHelpEngine *helpEngine, QWidget *parent) :
    QTextBrowser(parent),
    m_helpEngine(helpEngine)
{
}



void HelpTextBrowser::setSource(const QUrl &url)
{
    if (!url.toString().startsWith(QStringLiteral("http")))
        QTextBrowser::setSource(url);

    else if (m_openExternalLinksEnabled)
        QDesktopServices::openUrl(url);
}



QVariant HelpTextBrowser::loadResource(const int type, const QUrl &name)
{
    if (name.scheme() == QStringLiteral("qthelp"))
        return m_helpEngine->fileData(name);

    return QTextBrowser::loadResource(type, name);
}



void HelpTextBrowser::wheelEvent(QWheelEvent *event)
{
    if (!event->modifiers().testFlag(Qt::ControlModifier))
        QTextBrowser::wheelEvent(event);
}
