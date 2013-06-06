#ifndef NETMUSICWIDGET_H
#define NETMUSICWIDGET_H

#include <QtGui>
#include <QtCore>
#include "button.h"
#include "label.h"
#include "tipdata.h"
#include "timer.h"
#include "contactlabel.h"

/*下推音乐*/
class NetmusicWidget :public QGraphicsWidget
{
    Q_OBJECT
public:
    NetmusicWidget();
    ~NetmusicWidget();
    void netmusicWidgetUpdate(TipMessage *message);
    void netmusicWidgetKillTimer();
    bool netmusicIsRuning();
    void netmusicReadTip();
private slots:
    void netmusicLeftButtonClick();
    void netmusicRightButtonClick();
    void netmusicTimerTimeout();
    void netmusicThreadFinished();
signals:
    void noVisible();
private:
    QGraphicsPixmapItem *bg;
    QGraphicsPixmapItem *bgs;
    QGraphicsPixmapItem *icon;
    Button *leftButton;
    Button *rightButton;
    Label *title;
    Label *name;
    int id;
    Timer *timer;
    ContactLabel *sureLabel;
    ContactLabel *cancelLabel;
};


#endif // NETMUSICWIDGET_H
