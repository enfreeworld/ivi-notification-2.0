#include "netmusicwidget.h"
#include "tipthread.h"
#include "filepath.h"

NetmusicWidget::NetmusicWidget():
    QGraphicsWidget()
{
    bg = new QGraphicsPixmapItem(QPixmap(IMAGE_PATH"/bg_new.png"),this);
    bgs =new QGraphicsPixmapItem(QPixmap(IMAGE_PATH"/bgs.png"),this);
    icon = new QGraphicsPixmapItem(QPixmap(IMAGE_PATH"/bg.png"),bgs);
    bgs->setPos(50,20);

    title = new Label(this);
    title->setFont(QFont("Sans",18));
    title->setText(QObject::tr("Recive Music !"));
    title->setPos(220,20);

    name = new Label(this);
    name->setFont(QFont("Sans",18));
    name->setText("离歌");
    name->setPos(220,80);

    leftButton = new Button(QPixmap(IMAGE_PATH"/textbg_button.png"),QPixmap(IMAGE_PATH"/textbg_button_down.png"),this);
    leftButton->setPos(20,160);
    connect(leftButton,SIGNAL(clicked()),this,SLOT(netmusicLeftButtonClick()));
    rightButton = new Button(QPixmap(IMAGE_PATH"/textbg_button.png"),QPixmap(IMAGE_PATH"/textbg_button_down.png"),this);
    rightButton->setPos(310,160);
    connect(rightButton,SIGNAL(clicked()),this,SLOT(netmusicRightButtonClick()));

    sureLabel = new ContactLabel(leftButton);
    sureLabel->setText(QObject::tr("Sure"));
    sureLabel->setGeometry(0,0,200,80);
    sureLabel->setFont(QFont("Sans",16));
    sureLabel->setPos(60,15);

    cancelLabel = new ContactLabel(rightButton);
    cancelLabel->setText(QObject::tr("Cancel"));
    cancelLabel->setGeometry(0,0,200,80);
    cancelLabel->setFont(QFont("Sans",16));
    cancelLabel->setPos(60,15);

    timer = new Timer(7000);
    connect(timer,SIGNAL(timerOut()),this,SLOT(netmusicTimerTimeout()));
    id = 0;
}


NetmusicWidget::~NetmusicWidget()
{
    delete bg;
    delete icon;
    delete bgs;
    delete title;
    delete name;
    delete leftButton;
    delete rightButton;
    delete timer;
}

void NetmusicWidget::netmusicWidgetUpdate(TipMessage *message)
{
    if(id !=0)
    {
        killTimer(id);
        id = 0;
    }


    id = timer->startTimer(7000);    
    netmusicReadTip();
}

void NetmusicWidget::netmusicTimerTimeout()
{
    if(id !=0)
    {
        killTimer(id);
        id = 0;
    }

    emit noVisible();

    qDebug()<<"-----netmusic";
}

void NetmusicWidget::netmusicWidgetKillTimer()
{
    if(id !=0)
    {
        killTimer(id);
        id = 0;
    }
}

void NetmusicWidget::netmusicLeftButtonClick()
{
    qDebug()<<"sure";
    if(netmusicIsRuning())
    {
        system("dbus-send / com.asianux.netmusic.push_music ");
    }
    else
    {
    //    system("/usr/bin/netmusic push_music &");
        TipThread *thread = new TipThread(QString("/usr/bin/netmusic push_music "));
        thread->start();
        connect(thread,SIGNAL(finished()),this,SLOT(netmusicThreadFinished()));
    }

    if(id !=0)
    {
        killTimer(id);
        id = 0;
    }

    emit noVisible();
}

void NetmusicWidget::netmusicRightButtonClick()
{
    qDebug()<<"cancel";
    if(id !=0)
    {
        killTimer(id);
        id = 0;
    }

    emit noVisible();

}

bool NetmusicWidget::netmusicIsRuning()
{
    char buf[128];
    FILE *fp;
    if ((fp = popen("ps ax | grep netmusic | grep -v grep", "r")) == NULL)
    {
        printf("popen() error\n");
        exit(1);
    }

    while (fgets(buf,sizeof(buf),fp))
    {
        // printf("-------------%s",buf);
    }
    pclose(fp);

    if (strstr(buf,"netmusic") == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void NetmusicWidget::netmusicReadTip()
{
    system("dbus-send / com.asianux.ttserver.tts string:SYSTEM string:下推歌曲已收到，请注意查收。&");
}

void NetmusicWidget::netmusicThreadFinished()
{
    QObject* obj = sender();
    TipThread * th = qobject_cast<TipThread*>(obj);
    obj->deleteLater();
}
