#include "timer.h"


Timer::Timer(int duration):
    QObject()
{
    id = 0;
}

Timer::~Timer()
{
    if(id)
        killTimer(id);
}

void Timer::timerRun(int duration)
{
    if(id)
    {
        killTimer(id);
        id=0;
    }

    id = startTimer(duration);
}


void Timer::timerStop()
{
    if(id)
    {
        killTimer(id);
        id=0;
    }
}


void Timer::timerEvent(QTimerEvent *event)
{
    emit timerOut();
    //qDebug()<<"timerOut";
}
