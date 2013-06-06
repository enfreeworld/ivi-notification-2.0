#ifndef NETERRORWIDGET_H
#define NETERRORWIDGET_H

#include <QtGui>
#include <QtCore>
#include "timer.h"
#include "tipdata.h"

/*下推失败提示*/

class NeterrorWidget : public QGraphicsWidget
{
    Q_OBJECT
public:
    NeterrorWidget();
    ~NeterrorWidget();
    void neterrorWidgetUpdate(TipMessage *message);
    void neterrorWidgetKillTimer();
private slots:
    void neterrorTimerTimeout();
signals:
    void noVisible();
private:
    QGraphicsPixmapItem *bg;
    QGraphicsPixmapItem *tipText;
    int id;
    Timer *timer;
};

#endif // NETERRORWIDGET_H
