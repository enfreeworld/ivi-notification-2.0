#ifndef NETVIDEOWIDGET_H
#define NETVIDEOWIDGET_H

#include <QtGui>
#include <QtCore>
#include "button.h"
#include "label.h"
#include "timer.h"
#include "tipdata.h"
#include "contactlabel.h"

/*下推视频*/
class NetvideoWidget: public QGraphicsWidget
{
    Q_OBJECT
public:
    NetvideoWidget();
    ~NetvideoWidget();
    void netvideoWidgetUpdate(TipMessage *message);
    void netvideoWidgetKillTimer();
    bool netvideoIsRunning();
    void netvideoReadTip();
private slots:
    void netvideoLeftButtonClick();
    void netvideoRightButtonClick();
    void netvideoTimerTimeout();
    void netvideoThreadFinished();
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

#endif // NETVIDEOWIDGET_H
