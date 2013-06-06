#include "neterrorwidget.h"
#include "filepath.h"

NeterrorWidget::NeterrorWidget():
    QGraphicsWidget()
{
    bg = new QGraphicsPixmapItem(QPixmap(IMAGE_PATH"/search_BG.png"),this);
    tipText = new QGraphicsPixmapItem(QPixmap(IMAGE_PATH"/data/neterror.png"),bg);

    tipText->setPos(70,70);
    timer = new Timer(7000);
    connect(timer,SIGNAL(timerOut()),this,SLOT(neterrorTimerTimeout()));
    id=0;

}

NeterrorWidget::~NeterrorWidget()
{
    delete tipText;
    delete bg;
    delete timer;
}

void NeterrorWidget::neterrorWidgetUpdate(TipMessage *message)
{
    if(id !=0)
    {
        killTimer(id);
        id = 0;
    }

    id = timer->startTimer(7000);

}

void NeterrorWidget::neterrorTimerTimeout()
{
    if(id !=0)
    {
        killTimer(id);
        id = 0;
    }

    emit noVisible();

    qDebug()<<"---neterror";
}

void NeterrorWidget::neterrorWidgetKillTimer()
{
    if(id !=0)
    {
        killTimer(id);
        id = 0;
    }
}
