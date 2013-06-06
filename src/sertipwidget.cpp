#include "sertipwidget.h"
#include "filepath.h"

SertipWidget::SertipWidget():
    QGraphicsWidget()
{
    bg = new QGraphicsPixmapItem(QPixmap(IMAGE_PATH"/search_BG.png"),this);
    title = new Label(bg);
    title->setFont(QFont("Sans",18));
    title->setText("您已欠费");
    title->setPos(180,90);
    timer = new Timer(7000);
    connect(timer,SIGNAL(timerOut()),this,SLOT(sertipTimerTimeout()));
    id= 0;
}

SertipWidget::~SertipWidget()
{
    delete title;
    delete bg;
    delete timer;
}

void SertipWidget::sertipWidgetUpdate(TipMessage *message)
{
    if(id !=0)
    {
        killTimer(id);
        id=0;
    }
    id = timer->startTimer(7000);   
}

void SertipWidget::sertipTimerTimeout()
{
    if(id !=0)
    {
        killTimer(id);
        id=0;
    }

    emit noVisible();

    qDebug()<<"----sertip";
}


void SertipWidget::sertipWidgetKillTimer()
{
    if(id !=0)
    {
        killTimer(id);
        id=0;
    }
}
