#include <QDesktopServices>
#include "helptextbrowser.h"



HelpTextBrowser::HelpTextBrowser(QHelpEngine *helpEngine, QWidget *parent) :
    QTextBrowser(parent),
    helpEngine(helpEngine)
{}



void HelpTextBrowser::setSource(const QUrl &url)
{
    if (!url.toString().startsWith("http"))
        QTextBrowser::setSource(url);

    else if (m_openExternalLinks)
        QDesktopServices::openUrl(url);
}



QVariant HelpTextBrowser::loadResource(int type, const QUrl &name)
{
    if (name.scheme() == "qthelp")
        return QVariant(helpEngine->fileData(name));

    return QTextBrowser::loadResource(type, name);
}



void HelpTextBrowser::wheelEvent(QWheelEvent *event)
{
    if (!event->modifiers().testFlag(Qt::ControlModifier))
        QTextBrowser::wheelEvent(event);
}
