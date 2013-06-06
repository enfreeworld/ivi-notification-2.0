#include "smswidget.h"
#include "filepath.h"

SmsWidget::SmsWidget():
    QGraphicsWidget()
{
    this->resize(525,240);

    setMinimumWidth(525);
    setMinimumHeight(240);

    bg = new QGraphicsPixmapItem(QPixmap(IMAGE_PATH"/search_BG.png"),this);
    leftButton = new Button(QPixmap(IMAGE_PATH"/sound.png"),this);
    leftButton->setPos(-5,-3);
    rightButton = new Button(QPixmap(IMAGE_PATH"/play.png"),this);
    rightButton->setPos(355,-3);

    connect(leftButton,SIGNAL(clicked()),this,SLOT(smsLeftButtonClick()));
    connect(rightButton,SIGNAL(clicked()),this,SLOT(smsRightButtonClick()));

    sms = new QGraphicsPixmapItem(QPixmap(IMAGE_PATH"/sms_icon.png"),this);
    sms->setPos(130,20);

    number = new Label(this);
    number->setFont(QFont("Sans",15));
    number->setText("");
    //number->setPos(200,50);
    number->setGeometry(210,50,150,80);

    content = new Label(this);
    content->setFont(QFont("Sans",16));
    content->setText("");
    //content->setPos(150,100);
    content->setGeometry(150,100,180,60);
    content->setAlignment(Qt::AlignCenter);

    timer = new Timer(7000);
    connect(timer,SIGNAL(timerOut()),this,SLOT(smsTimerTimeout()));
    id = 0;
    smsMessage = NULL;
    smsDb = new SmsDb;

}

SmsWidget::~SmsWidget()
{
    delete bg;
    delete leftButton;
    delete rightButton;
    delete sms;
    delete number;
    delete content;
    delete timer;
    delete smsDb;
}

void SmsWidget::smsLeftButtonClick()
{
    if(id !=0)
    {
        killTimer(id);
        id = 0;
    }

    number->setText("");
    content->setText("");

    char *commond = (char *)malloc((1024)*(sizeof(char *)));

    sprintf(commond,"dbus-send / com.asianux.ttserver.tts string:\"SYSTEM\" string:\"%s.\"",
            qPrintable(smsMessage->content1));
    system(commond);
    free(commond);
    commond = NULL;

    smsDb->smsDbUpdate(smsMessage);

    emit noVisible();
}

void SmsWidget::smsRightButtonClick()
{
    system("dbus-send --session --type=signal --dest=com.asianux.sms / com.asianux.sms.showwindow");
    system("dbus-send / com.asianux.sms.newmessage");
    if(id !=0)
    {
        killTimer(id);
        id = 0;
    }

    number->setText("");
    content->setText("");

    emit noVisible();
}


void SmsWidget::smsWidgetSmssend(QString str)
{
    if(smsIsRunning())
    {
        system(" dbus-send / com.asianux.sms.showwindow ");
        char *smssend = (char *)malloc(128*(sizeof(char *)));

        qDebug()<<str;

        sprintf(smssend,"dbus-send / com.asianux.sms.smssend string:%s &",qPrintable(str));
        system(smssend);

        free(smssend);
        smssend = NULL;

    }
    else
    {
        TipThread *thread = new TipThread(QString("sms send %1").arg(str));
        thread->start();
        connect(thread,SIGNAL(finished()),this,SLOT(smsWidgetThreadFinished()));
    }
}

void SmsWidget::smsWidgetThreadFinished()
{
    QObject* obj = sender();
    TipThread * th = qobject_cast<TipThread*>(obj);
    obj->deleteLater();
}

void SmsWidget::smsTimerTimeout()
{
    if(id !=0)
    {
        killTimer(id);
        id = 0;
    }

    number->setText("");
    content->setText("");

    emit noVisible();
}

void SmsWidget::smsWidgetKillTimer()
{
    if(id !=0)
    {
        killTimer(id);
        id = 0;
    }

    number->setText("");
    content->setText("");

}

void SmsWidget::smsWidgetUpdate(TipMessage *message)
{
    if(id !=0)
    {
        killTimer(id);
        id = 0;
    }
    qDebug()<<message->number;
    id = timer->startTimer(7000);
    number->setText(message->number);
    content->setText(message->content1);
    smsMessage = message;
    smsReadTip();
}

bool SmsWidget::smsIsRunning()
{
    char buf[128];
    FILE *fp;
    if ((fp = popen("ps ax | grep sms | grep -v grep", "r")) == NULL)
    {
        printf("popen() error\n");
        exit(1);
    }

    while (fgets(buf,sizeof(buf),fp))
    {
        // printf("-------------%s",buf);
    }

    pclose(fp);

    if (strstr(buf,"sms") == 0)
    {
        return false;
    }
    else
    {
        if(strstr(buf,"smsterm") == NULL)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}


void SmsWidget::smsReadTip()
{
    system("dbus-send / com.asianux.ttserver.tts string:SYSTEM string:\"收到新短消息，请注意查收。\" ");
    system("dbus-send / com.asianux.sms.update");

}

void SmsWidget::smsWidgetSetName(QString str)
{
    if(str.compare("Unknown") != 0)
        number->setText(str);
}
