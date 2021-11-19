#ifndef HELPTOOLBAR_H
#define HELPTOOLBAR_H

#include <QToolBar>
#include <QEvent>

class HelpToolBar : public QToolBar
{
    Q_OBJECT

public:
    explicit HelpToolBar(QWidget *parent = nullptr);

    void setBackwardEnabled(const bool enabled);
    void setForwardEnabled(const bool enabled);

    void retranslate();

protected:
    virtual void changeEvent(QEvent *event) override;

signals:
    void contentsTriggered();
    void homeTriggered();
    void backwardTriggered();
    void forwardTriggered();

private:
    QAction *m_contentsAction;
    QAction *m_homeAction;
    QAction *m_backwardAction;
    QAction *m_forwardAction;
};

#endif // HELPTOOLBAR_H
