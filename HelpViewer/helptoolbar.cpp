#include "helptoolbar.h"



HelpToolBar::HelpToolBar(QWidget *parent) : QToolBar(parent)
{
    setFloatable(false);
    setContextMenuPolicy(Qt::PreventContextMenu);

    setStyleSheet(QStringLiteral("QToolBar {padding: 5}"
                                 "QToolBar QToolButton {padding: 2; margin: 0}"));

    m_contentsAction = addAction(QIcon(QStringLiteral(":/images/icon-menu.svg")), QString(),
                                 this, &HelpToolBar::contentsTriggered);

    addSeparator();

    m_homeAction = addAction(QIcon(QStringLiteral(":/images/icon-home.svg")), QString(),
                             this, &HelpToolBar::homeTriggered);

    m_backwardAction = addAction(QIcon(QStringLiteral(":/images/icon-arrow-left.svg")), QString(),
                                 this, &HelpToolBar::backwardTriggered);

    m_forwardAction = addAction(QIcon(QStringLiteral(":/images/icon-arrow-right.svg")), QString(),
                                this, &HelpToolBar::forwardTriggered);

    m_contentsAction->setShortcut(Qt::Key_F2);
    m_backwardAction->setShortcut(QKeySequence::MoveToPreviousPage);
    m_homeAction->setShortcut(QKeySequence::MoveToStartOfLine);
    m_forwardAction->setShortcut(QKeySequence::MoveToNextPage);

    retranslate();
}



void HelpToolBar::setBackwardEnabled(const bool enabled)
{
    m_backwardAction->setEnabled(enabled);
}



void HelpToolBar::setForwardEnabled(const bool enabled)
{
    m_forwardAction->setEnabled(enabled);
}



void HelpToolBar::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
        retranslate();

    QToolBar::changeEvent(event);
}



void HelpToolBar::retranslate()
{
    m_contentsAction->setText(tr("Show/hide contents"));
    m_homeAction->setText(tr("Home"));
    m_backwardAction->setText(tr("Backward"));
    m_forwardAction->setText(tr("Forward"));
}
