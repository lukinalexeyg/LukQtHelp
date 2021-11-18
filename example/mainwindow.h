#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "helpviewer.h"

#include <QMainWindow>
#include <QCloseEvent>
#include <QHBoxLayout>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void closeEvent(QCloseEvent*);

private:
    HelpViewer *m_helpViewer;
    QHBoxLayout *m_hBoxLayout;

    template<typename Func>
    void addPushButton(const QString &name, Func slot);
};

#endif // MAINWINDOW_H
