#include <stdio.h>
#include <stdlib.h>
#include "dialwidget.h"
#include "dataresource.h"
#include "tipthread.h"
#include "filepath.h"


DialWidget::DialWidget():
    QGraphicsWidget()
{
    this->resize(525,240);

    setMinimumWidth(525);
    setMinimumHeight(240);

    bg = new QGraphicsPixmapItem(QPixmap(IMAGE_PATH"/search_BG.png"),this);
    leftButton = new Button(QPixmap(IMAGE_PATH"/call.png"),QPixmap(IMAGE_PATH"/call2.png"),this);
    leftButton->setPos(-5,-5);
    rightButton = new Button(QPixmap(IMAGE_PATH"/hang.png"),QPixmap(IMAGE_PATH"/hang2.png"),this);
    rightButton->setPos(355,-5);

    connect(leftButton,SIGNAL(clicked()),this,SLOT(dialLeftButtonClick()));
    connect(rightButton,SIGNAL(clicked()),this,SLOT(dialRightButtonClick()));

    phoneAddress = new Label(this);
    phoneAddress->setText(QObject::tr("Unknown Address"));
    phoneAddress->setFont(QFont("Sans",16));
    phoneAddress->setGeometry(123,20,250,60);
    phoneAddress->setAlignment(Qt::AlignCenter);

    name = new Label(this);
    name->setText(QObject::tr("Unknown"));
    name->setFont(QFont("Sans",16));
    //name->setPos(200,40);
    name->setGeometry(133,70,230,60);
    name->setAlignment(Qt::AlignCenter);

    number = new Label(this);
    number->setText("18661018857");
    number->setFont(QFont("Sans",16));
    number->setGeometry(133,110,230,60);
    number->setAlignment(Qt::AlignCenter);

    ecallNumber = Resource::stringValue("default/ecallno");
    icallNumber = Resource::stringValue("default/icallno");

    telephoneDb = new TelephoneDb();

    is_calling = false;
}

DialWidget::~DialWidget()
{
    delete bg;
    delete leftButton;
    delete rightButton;
    delete phoneAddress;
    delete name;
    delete number;
    delete telephoneDb;
}


void DialWidget::dialLeftButtonClick()
{
    emit noVisible();

    if(telephoneIsRunning())
    {
        system("dbus-send --session --type=signal --dest=com.asianux.phone / com.asianux.phone.showring");//------change to showring

        char *start = (char *)malloc(512*(sizeof(char *)));
        memset(start,'0',strlen(start));

        sprintf(start,"dbus-send / com.asianux.phone.ring string:%s",qPrintable(phoneNumber));

        system(start);
        free(start);
        start = NULL;

    }
    else
    {
        TipThread *thread = new TipThread(QString("telephone ring %1").arg(phoneNumber));
        thread->start();
        connect(thread,SIGNAL(finished()),this,SLOT(dialWidgetThreadFinished()));

    }

    system("dbus-send / com.asianux.pdaemon.answer " );

    SmsPlay *smsPlay = SmsPlay::instance();
    smsPlay->sms_play_set_stop();

    is_calling = true;

    name->setText(QObject::tr("Unknown"));
    phoneAddress->setText(QObject::tr("Unknown Address"));
}

void DialWidget::dialRightButtonClick()
{
    emit noVisible();

    telephoneDb->telephoneDbUpdate(phoneNumber);

    system("dbus-send / com.asianux.pdaemon.reject ");

    system("dbus-send / com.asianux.phone.refreshrecords ");

    SmsPlay *smsPlay = SmsPlay::instance();
    smsPlay->sms_play_set_stop();

    is_calling = false;

    name->setText(QObject::tr("Unknown"));
    phoneAddress->setText(QObject::tr("Unknown Address"));

}

void DialWidget::dialWidgetUpdate(TipMessage *message)
{
    phoneNumber = message->number;
    number->setText(phoneNumber);
}

void DialWidget::dialWidgetSetName(QString str)
{
    if(str.compare("Unknown") == 0)
    {
        qDebug()<<"1";
        name->setText(QObject::tr("Unknown"));
    }
    else
    {
        qDebug()<<"2";
        name->setText(str);
    }
}

void DialWidget::dialWidgetSetAddress(QString str)
{
    phoneAddress->setText(str);
}

bool DialWidget::telephoneIsRunning()
{
    char buf[128];
    FILE *fp;
    if ((fp = popen("ps ax | grep telephone | grep -v grep", "r")) == NULL)
    {
        printf("popen() error\n");
        exit(1);
    }

    while (fgets(buf,sizeof(buf),fp))
    {
        // printf("-------------%s",buf);
    }
    pclose(fp);

    qDebug()<<buf;

    if (strstr(buf,"telephone") == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void DialWidget::dialWidgetEcall()
{
    if(telephoneIsRunning())
    {
        system("dbus-send --session --type=signal --dest=com.asianux.phone / com.asianux.phone.showring");
        system("dbus-send --session --type=signal --dest=com.asianux.phone / com.asianux.phone.ecall");
    }
    else
    {
        TipThread *thread = new TipThread(QString("telephone ring %1").arg(phoneNumber));
        thread->start();
        connect(thread,SIGNAL(finished()),this,SLOT(dialWidgetThreadFinished()));

    }
}

void DialWidget::dialWidgetIcall()
{
    if(telephoneIsRunning())
    {
        system("dbus-send --session --type=signal --dest=com.asianux.phone / com.asianux.phone.showring");
        system("dbus-send --session --type=signal --dest=com.asianux.phone / com.asianux.phone.icall");
    }
    else
    {
        TipThread *thread = new TipThread(QString("telephone ring %1").arg(phoneNumber));
        thread->start();
        connect(thread,SIGNAL(finished()),this,SLOT(dialWidgetThreadFinished()));
    }
}

void DialWidget::dialWidgetSmscall(QString str)
{
    qDebug()<<"\n----str "<<str;
    if(telephoneIsRunning())
    {
        system(" dbus-send / com.asianux.phone.showring ");
        char *smscall = (char *)malloc(128*(sizeof(char *)));

        qDebug()<<str;

        sprintf(smscall,"dbus-send / com.asianux.phone.call string:%s &",qPrintable(str));
        system(smscall);

        free(smscall);
        smscall = NULL;
    }
    else
    {
        TipThread *thread = new TipThread(QString("telephone call %1").arg(str));
        thread->start();
        connect(thread,SIGNAL(finished()),this,SLOT(dialWidgetThreadFinished()));

    }
}

void DialWidget::dialWidgetThreadFinished()
{
    QObject* obj = sender();
    TipThread * th = qobject_cast<TipThread*>(obj);
    obj->deleteLater();

}
