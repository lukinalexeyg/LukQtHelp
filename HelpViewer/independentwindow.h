#ifndef INSTANTDELETEDWINDOW_H
#define INSTANTDELETEDWINDOW_H

#pragma once

#include <QMainWindow>
#include <QCloseEvent>

class IndependentWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit IndependentWindow(QWidget *parent = nullptr);
    void setSize(QSize size, QWidget *basisWidget = Q_NULLPTR);

signals:
    void closed();

protected:
   void closeEvent(QCloseEvent *event);
};

#endif // INSTANTDELETEDWINDOW_H
