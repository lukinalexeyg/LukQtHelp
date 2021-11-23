#ifndef INDEPENDENTWINDOW_H
#define INDEPENDENTWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>

class IndependentWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit IndependentWindow(QWidget *parent = nullptr);

    void setSize(const QSize &size, QWidget *basisWidget = nullptr);

signals:
    void closed();

protected:
   void closeEvent(QCloseEvent *event);
};

#endif // INDEPENDENTWINDOW_H
