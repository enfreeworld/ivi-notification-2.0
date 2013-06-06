#include  "tipthread.h"

TipThread::TipThread(QString str):
    QThread(),commond(str)
{

}

TipThread::~TipThread()
{

}

void TipThread::run()
{
    qDebug()<<commond;
   system(qPrintable(commond));
}
