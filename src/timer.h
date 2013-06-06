#ifndef TIMER_H
#define TIMER_H

#include <QtCore>

class Timer : public QObject
{
    Q_OBJECT
public:
    Timer(int duration);
    ~Timer();
    void timerStop();
    void timerRun(int duration);
protected:
    void timerEvent(QTimerEvent *event);
signals:
    void timerOut();
private:
    int id;
};

#endif // TIMER_H
