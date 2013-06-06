#ifndef SERTIPWIDGET_H
#define SERTIPWIDGET_H

#include <QtGui>
#include <QtCore>
#include "label.h"
#include "timer.h"
#include "tipdata.h"

/*欠费提示*/
class SertipWidget : public QGraphicsWidget
{
    Q_OBJECT
public:
    SertipWidget();
    ~SertipWidget();
    void sertipWidgetUpdate(TipMessage *message);
    void sertipWidgetKillTimer();
private slots:
    void sertipTimerTimeout();
signals:
    void noVisible();
private:
    QGraphicsPixmapItem *bg;
    Label *title;
    int id;
    Timer *timer;

};

#endif // SERTIPWIDGET_H
