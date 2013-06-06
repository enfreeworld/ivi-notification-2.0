#ifndef TIPTHREAD_H
#define TIPTHREAD_H

#include <QtCore>

class TipThread :public QThread
{
    Q_OBJECT
public:
    TipThread(QString str);
    ~TipThread();
protected:
    void run();
private:
    QString commond;
};

#endif // TIPTHREAD_H
