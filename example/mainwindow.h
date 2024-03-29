#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "lhelpviewer.h"

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
    LHelpViewer *m_helpViewer;

    template<typename Func>
    void addPushButton(QHBoxLayout *hBoxLayout, const QString &name, Func func);
};

#endif // MAINWINDOW_H
