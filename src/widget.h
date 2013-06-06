#ifndef WIDGET_H
#define WIDGET_H

#include <QtGui>
#include <QtDBus>
#include "tipwindow.h"

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget*p = NULL, Qt::WindowFlags f = 0);
    ~Widget();
private slots:
    void widgetShowWindow();
    void widgetHideWindow();
private:
    TipWindow *tipWindow;

protected:
    void closeEvent(QCloseEvent *e);
};

#endif // WIDGET_H
