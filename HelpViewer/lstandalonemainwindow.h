#ifndef LSTANDALONEMAINWINDOW_H
#define LSTANDALONEMAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>

class LStandaloneMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LStandaloneMainWindow(QWidget *parent = nullptr);

    void setSize(const QSize &size, QWidget *basisWidget = nullptr);

signals:
    void closed();

protected:
   void closeEvent(QCloseEvent *event);
};

#endif // LSTANDALONEMAINWINDOW_H
