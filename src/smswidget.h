#ifndef SMSWIDGET_H
#define SMSWIDGET_H

#include <QtGui>
#include <QtCore>
#include "button.h"
#include "label.h"
#include "timer.h"
#include "tipdata.h"
#include "tipdb.h"
#include "tipthread.h"

/*短信提示*/
class SmsWidget:public QGraphicsWidget
{
    Q_OBJECT
public:
    SmsWidget();
    ~SmsWidget();
    void smsWidgetUpdate(TipMessage *message);
    void smsWidgetKillTimer();
    bool smsIsRunning();
    void smsReadTip();
    void smsWidgetSetName(QString str);
    void smsWidgetSmssend(QString str);

private slots:
    void smsLeftButtonClick();
    void smsRightButtonClick();
    void smsTimerTimeout();
    void smsWidgetThreadFinished();

signals:
    void noVisible();
private:
    QGraphicsPixmapItem *bg;
    QGraphicsPixmapItem *sms;
    Button *leftButton;
    Button *rightButton;
    Label *number;
    Label *content;
    int id;
    Timer *timer;
    SmsDb *smsDb;
    TipMessage *smsMessage;
};

#endif // SMSWIDGET_H
