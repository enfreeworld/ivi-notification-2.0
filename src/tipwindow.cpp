#include "tipwindow.h"

QDBusConnection TipWindow::bus=QDBusConnection::sessionBus();

TipWindow::TipWindow(QWidget *parent):
    QGraphicsView(parent)
{
    smsMessage = NULL;
    musicMessage = NULL;
    videoMessage = NULL;
    errorMessage = NULL;
    sertipMessgae = NULL;

    windowVisable = false;

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tipWindowBgUpdate();
    setCacheMode(QGraphicsView::CacheBackground);
    qDebug()<<"init";

    /*( while(QCoreApplication::hasPendingEvents ())
    {
        QCoreApplication::processEvents();
    }
    */

    QGraphicsScene *scene = new QGraphicsScene;
    this->setScene(scene);
    scene->setSceneRect(0,0,525,240);

    this->setStyleSheet("background: transparent;border:0px");

    setMinimumSize(525,240);
    setMaximumSize(525,240);
    this->move(150,120);

    SmsPlay *smsplay = SmsPlay::instance();

    dialWidget = new DialWidget();
    scene->addItem(dialWidget);
    dialWidget->setPos(0,0);
    dialWidget->hide();
    connect(dialWidget,SIGNAL(noVisible()),this,SLOT(tipWindowWidgetNovisible()));

    smsWidget = new SmsWidget();
    scene->addItem(smsWidget);
    smsWidget->setPos(0,0);
    smsWidget->hide();
    connect(smsWidget,SIGNAL(noVisible()),this,SLOT(tipWindowWidgetNovisible()));

    netvideoWidget = new NetvideoWidget();
    scene->addItem(netvideoWidget);
    netvideoWidget->setPos(0,0);
    netvideoWidget->hide();
    connect(netvideoWidget,SIGNAL(noVisible()),this,SLOT(tipWindowWidgetNovisible()));

    netmusicWidget = new NetmusicWidget();
    scene->addItem(netmusicWidget);
    netmusicWidget->setPos(0,0);
    netmusicWidget->hide();
    connect(netmusicWidget,SIGNAL(noVisible()),this,SLOT(tipWindowWidgetNovisible()));

    neterrorWidget = new NeterrorWidget();
    scene->addItem(neterrorWidget);
    neterrorWidget->setPos(0,0);
    neterrorWidget->hide();
    connect(neterrorWidget,SIGNAL(noVisible()),this,SLOT(tipWindowWidgetNovisible()));

    sertipWidget = new SertipWidget();
    scene->addItem(sertipWidget);
    sertipWidget->setPos(0,0);
    sertipWidget->hide();
    connect(sertipWidget,SIGNAL(noVisible()),this,SLOT(tipWindowWidgetNovisible()));

    timer = new Timer(7000);
    timer->startTimer(7000);
    connect(timer,SIGNAL(timerOut()),this,SLOT(tipWindowCheckMessage()));

    contactDb = new ContactDb;
    phoneAddressDb = new PhoneAddressDb;

    calling = false;
    windowVisable = false;

    QDBusConnection::sessionBus().connect(QString(),QString(),
                                          "com.asianux.notification","notify",
                                          this,SLOT(tipWindowDbusNotify(QString,QString,QString,QString)));
    /*
    QDBusConnection::sessionBus().connect(QString(),QString(),
                                          "com.asianux.notification","close",this,SLOT(tipWindowDbusClose()));

    QDBusConnection::sessionBus().connect(QString(),QString(),
                                          "com.asianux.notification","panel",this,SLOT(tipWindowDbusPanel()));*/

    QDBusConnection::sessionBus().connect(QString(),QString(),
                                          "com.asianux.notification","icall",this,SLOT(tipWindowDbusIcall()));

    QDBusConnection::sessionBus().connect(QString(),QString(),
                                          "com.asianux.notification","ecall",this,SLOT(tipWindowDbusEcall()));

    QDBusConnection::sessionBus().connect(QString(),QString(),
                                          "com.asianux.notification","hook",this,SLOT(tipWindowDbusHook()));

    QDBusConnection::sessionBus().connect(QString(),QString(),
                                          "com.asianux.notification","smscall",this,SLOT(tipWindowDbusSmscall(QString)));

    QDBusConnection::sessionBus().connect(QString(),QString(),
                                          "com.asianux.notification","smssend",this,SLOT(tipWindowDbusSmssend(QString)));

    QDBusConnection::sessionBus().connect(QString(),QString(),
                                          "com.asianux.notification","tiphide",this,SLOT(tipWindowDbusTiphide()));

    QDBusConnection::sessionBus().connect(QString(),QString(),
                                          "com.asianux.notification","tipshow",this,SLOT(tipWindowDbusTipshow()));

    QDBusConnection::sessionBus().connect(QString(),QString(),
                                          "com.asianux.notification","call",this,SLOT(tipWindowPhoneDbusCall(QString)));

    QDBusConnection::sessionBus().connect(QString(),QString(),
                                          "com.asianux.notification","hangup",this,SLOT(tipWindowPhoneDbusHook()));

    QDBusConnection::sessionBus().connect(QString(),QString(),
                                          "com.asianux.notification","answer",this,SLOT(tipWindowPhoneDbusAnswer()));

    QDBusConnection::sessionBus().connect(QString(),QString(),
                                          "com.asianux.notification","reject",this,SLOT(tipWindowPhoneDbusReject()));

    QDBusConnection::sessionBus().connect(QString(),QString(),
                                          "com.asianux.notification","extension",this,SLOT(tipWindowPhoneDbusExtension(QString)));

    QDBusConnection::sessionBus().connect(QString(),QString(),
                                          "com.asianux.notification","begin",this,SLOT(tipWindowServerDbusDialBegin()));

    QDBusConnection::sessionBus().connect(QString(),QString(),
                                          "com.asianux.notification","end",this,SLOT(tipWindowServerDbusDialEnd()));

    curType = TIP_SRC_NONE;
}

TipWindow::~TipWindow()
{
    delete dialWidget;
    delete smsWidget;
    delete netvideoWidget;
    delete netmusicWidget;
    delete neterrorWidget;
    delete sertipWidget;
    delete timer;
    delete contactDb;
    delete phoneAddressDb;
}

void TipWindow::tipWindowCheckMessage()
{
    if(curType == TIP_SRC_DIAL)
    {
        newSms = false;
        newVideo = false;
        newMusic = false;
        newSertip = false;
        newError = false;
        return ;
    }
    else if(curType == TIP_SRC_SMS_FRIREQ)
    {
        return ;
    }
    else if(curType == TIP_SRC_SMS_FRIREC)
    {
        return ;
    }
    else
    {
        if(calling)
        {
            newSms = false;
            newVideo = false;
            newMusic = false;
            newSertip = false;
            newError = false;
            return ;
        }


        if(windowVisable)
            return ;

        if(newSms && smsMessage != NULL)
        {
            newSms = false;
            tipWindowUpdateSms(smsMessage);

            qDebug()<<"--------------sms";

            return ;
        }

        if(newVideo && videoMessage != NULL)
        {
            newVideo = false;
            tipWindowUpdateNetvideo(videoMessage);
            qDebug()<<"--------------video";
            return ;
        }

        if(newMusic && musicMessage != NULL)
        {
            newMusic = false;
            tipWindowUpdateNetmusic(musicMessage);
            qDebug()<<"--------------music";
            return ;
        }

        if(newError && errorMessage != NULL)
        {
            newError = false;
            tipWindowUpdateNeterror(errorMessage);
            qDebug()<<"--------------error";
            return ;
        }

        if(newSertip && sertipMessgae != NULL)
        {
            newSertip = false;
            tipWindowUpdateSertip(sertipMessgae);
            qDebug()<<"--------------sertip";
            return ;
        }
    }
}

void TipWindow::tipWindowUpdateReal(TipMessage *message)
{
    MessageType type = tipWindowMessageType(message->type);

    switch(type)
    {
    case TIP_SRC_DIAL:
    {
        tipWindowUpdateDial(message);
        curType = TIP_SRC_DIAL;
    }
        break;
    case TIP_SRC_SMS_COMMON:
    {
        if(smsMessage != NULL)
        {
            delete smsMessage;
            smsMessage = NULL;
        }

        if(!calling)
        {
            smsMessage = message;
            newSms = true;
        }
    }
        break;
    case TIP_SRC_SMS_FRIREQ:
    {
        curType = TIP_SRC_SMS_FRIREQ;
    }
        break;
    case TIP_SRC_SMS_FRIREC:
    {
        curType = TIP_SRC_SMS_FRIREC;
    }
        break;
    case TIP_SRC_SMS_NETMUSIC:
    {
        if(musicMessage != NULL)
        {
            delete musicMessage;
            musicMessage = NULL;
        }

        musicMessage = message;
        newMusic = true;
    }
        break;
    case TIP_SRC_SMS_NETMOVIE:
    {
        if(videoMessage != NULL)
        {
            delete videoMessage;
            videoMessage = NULL;
        }

        videoMessage = message;
        newVideo = true;
    }
        break;
    case TIP_SRC_SMS_SERTIP:
    {
        if(sertipMessgae != NULL)
        {
            delete sertipMessgae;
            sertipMessgae = NULL;
        }

        sertipMessgae = message;
        newSertip = true;

    }
        break;
    case TIP_SRC_SMS_NETERROR:
    {
        if(errorMessage != NULL)
        {
            delete errorMessage;
            errorMessage = NULL;
        }

        errorMessage = message;
        newError = true;
    }
        break;
    default:
        break;
    }
}

void TipWindow::tipWindowDbusNotify(QString str1, QString str2, QString str3, QString str4)
{
    if(!str1.isEmpty())
    {
        TipMessage *message = new TipMessage;
        message->type = str1;
        message->number = str2;
        message->content1 = str3;
        message->content2 = str4;

        tipWindowUpdateReal(message);
    }
}

MessageType TipWindow::tipWindowMessageType(QString str)
{
    if(str == "dial")
    {
        return TIP_SRC_DIAL;
    }
    else if(str == "sms")
    {
        return TIP_SRC_SMS_COMMON;
    }
    else if(str == "frireq")
    {
        return TIP_SRC_SMS_FRIREQ;
    }
    else if(str == "frirec")
    {
        return TIP_SRC_SMS_FRIREC;
    }
    else if(str == "netmusic")
    {
        return TIP_SRC_SMS_NETMUSIC;
    }
    else if(str == "netvideo")
    {
        return TIP_SRC_SMS_NETMOVIE;
    }
    else if(str == "sertip")
    {
        return TIP_SRC_SMS_SERTIP;
    }
    else if(str == "neterror")
    {
        return TIP_SRC_SMS_NETERROR;
    }
    else
    {
        return TIP_SRC_NONE;
    }
}

void TipWindow::tipWindowUpdateDial(TipMessage *message)
{
    emit show();

    qDebug()<<"dial";
    dialWidget->show();
    netvideoWidget->hide();
    netmusicWidget->hide();
    neterrorWidget->hide();
    sertipWidget->hide();
    smsWidget->hide();

    netvideoWidget->netvideoWidgetKillTimer();
    netmusicWidget->netmusicWidgetKillTimer();
    neterrorWidget->neterrorWidgetKillTimer();
    sertipWidget->sertipWidgetKillTimer();
    smsWidget->smsWidgetKillTimer();

    dialWidget->dialWidgetUpdate(message);

    QString str = contactDb->getName(message->number);

    qDebug()<<"\n\nget name:"<<str;

    if(!str.isEmpty())
    {
        dialWidget->dialWidgetSetName(str);
    }

    QString str2 = phoneAddressDb->getPhoneAddress(message->number);

    if(!str2.isEmpty())
    {
        dialWidget->dialWidgetSetAddress(str2);
    }

    tipWindowBgUpdate();

    windowVisable = true;

    SmsPlay *smsPlay = SmsPlay::instance();
    smsPlay->sms_play_set_play("test");
}

void TipWindow::tipWindowUpdateSms(TipMessage *message)
{
    qDebug()<<"sms";
    curType = TIP_SRC_SMS_COMMON;
    netvideoWidget->hide();
    netmusicWidget->hide();
    neterrorWidget->hide();
    sertipWidget->hide();
    dialWidget->hide();

    qDebug()<<"\n calling is:"<<calling;
    if(calling)
        smsWidget->hide();
    else
        smsWidget->show();

    smsWidget->smsWidgetUpdate(message);

    QString str = contactDb->getName(message->number);

    if(!str.isEmpty())
    {
        smsWidget->smsWidgetSetName(str);
    }

    tipWindowBgUpdate();

    windowVisable = true;

    emit show();
}

void TipWindow::tipWindowUpdateNetmusic(TipMessage *message)
{
    qDebug()<<"music";
    curType = TIP_SRC_SMS_NETMUSIC;
    netvideoWidget->hide();
    neterrorWidget->hide();
    sertipWidget->hide();
    dialWidget->hide();
    smsWidget->hide();
    netmusicWidget->show();
    netmusicWidget->netmusicWidgetUpdate(message);

    tipWindowBgUpdate();

    windowVisable = true;

    emit show();

}

void TipWindow::tipWindowUpdateNetvideo(TipMessage *message)
{
    qDebug()<<"video";
    curType = TIP_SRC_SMS_NETMOVIE;
    netmusicWidget->hide();
    neterrorWidget->hide();
    smsWidget->hide();
    sertipWidget->hide();
    dialWidget->hide();
    netvideoWidget->show();
    netvideoWidget->netvideoWidgetUpdate(message);

    qDebug()<<"+++++++++++++++++++video";

    tipWindowBgUpdate();

    windowVisable = true;

    emit show();

}

void TipWindow::tipWindowUpdateNeterror(TipMessage *message)
{
    qDebug()<<"error";
    curType = TIP_SRC_SMS_NETERROR;
    netvideoWidget->hide();
    netmusicWidget->hide();
    smsWidget->hide();
    sertipWidget->hide();
    dialWidget->hide();
    neterrorWidget->show();
    neterrorWidget->neterrorWidgetUpdate(message);

    tipWindowBgUpdate();

    windowVisable = true;

    emit show();
}

void TipWindow::tipWindowUpdateSertip(TipMessage *message)
{
    qDebug()<<"sertip";
    netvideoWidget->hide();
    netmusicWidget->hide();
    neterrorWidget->hide();
    sertipWidget->hide();
    smsWidget->hide();
    sertipWidget->show();
    sertipWidget->sertipWidgetUpdate(message);

    tipWindowBgUpdate();

    windowVisable = true;
    emit show();
}

void TipWindow::tipWindowWidgetNovisible()
{
    emit hide();
    windowVisable = false;
    curType = TIP_SRC_NONE;
}

void TipWindow::tipWindowDbusClose()
{
    qDebug()<<"close";
}

void TipWindow::tipWindowDbusEcall()
{
    qDebug()<<"ecall";
    if(curType == TIP_SRC_DIAL)
    {
        dialWidget->dialRightButtonClick();
        dialWidget->dialWidgetEcall();
    }
    else
    {
        dialWidget->dialWidgetEcall();
    }
}

void TipWindow::tipWindowDbusHook()
{
    qDebug()<<"hook";
    if(curType == TIP_SRC_DIAL)
    {
        dialWidget->dialRightButtonClick();
    }
}

void TipWindow::tipWindowDbusIcall()
{
    qDebug()<<"icall";
    if(curType == TIP_SRC_DIAL)
        return ;

    dialWidget->dialWidgetIcall();
}

void TipWindow::tipWindowDbusPanel()
{
    qDebug()<<"panel";
    if(curType == TIP_SRC_DIAL)
    {
        qDebug()<<"接听电话";
        dialWidget->dialLeftButtonClick();
    }
    else
    {
        system("dbus-send --session --type=signal --dest=com.asianux.phone / com.asianux.phone.showwindow");
    }
}

void TipWindow::tipWindowDbusSmscall(QString str)
{
    qDebug()<<"2========"<<str;

    dialWidget->dialWidgetSmscall(str);
}


void TipWindow::tipWindowDbusSmssend(QString str)
{
    qDebug()<<"3========"<<str;

    smsWidget->smsWidgetSmssend(str);
}

void TipWindow::tipWindowBgUpdate()
{
    // setBackgroundBrush(QPixmap::grabWindow(QApplication::desktop()->winId(),150,200,465,224));
}

void TipWindow::tipWindowDbusTipshow()
{
    qDebug()<<"11\n";
    calling = false;   //结束通话，提示窗口显示
}

void TipWindow::tipWindowDbusTiphide()
{
    qDebug()<<"22\n";
    calling = true;   //正在通话，提示窗口隐藏
}


void TipWindow::tipWindowPhoneDbusCall(QString str)
{
    qDebug()<<"call"<<str;
    QDBusMessage msg = QDBusMessage::createSignal("/", "com.asianux.pdaemon", "call");
    msg<<str;
    if(!bus.send(msg))
    {
        qDebug("dbus send  call  failed\n");
        exit(-1);
    }

    SmsPlay *smsPlay = SmsPlay::instance();
    smsPlay->sms_play_set_play();

}

void TipWindow::tipWindowPhoneDbusHook()
{
    qDebug()<<"hook";
    QString str = "h";
    QDBusMessage msg = QDBusMessage::createSignal("/", "com.asianux.pdaemon", "hook");
    msg<<str;
    if(!bus.send(msg))
    {
        qDebug("dbus send  call  failed\n");
        exit(-1);
    }

    SmsPlay *smsPlay = SmsPlay::instance();
    smsPlay->sms_play_set_stop();

}

void TipWindow::tipWindowPhoneDbusAnswer()
{
    qDebug()<<"answer";
    QDBusMessage msg = QDBusMessage::createSignal("/", "com.asianux.pdaemon", "answer");
    if(!bus.send(msg))
    {
        qDebug("dbus send  call  failed\n");
        exit(-1);
    }

}

void TipWindow::tipWindowPhoneDbusReject()
{
    qDebug()<<"reject";
    QDBusMessage msg = QDBusMessage::createSignal("/", "com.asianux.pdaemon", "reject");
    if(!bus.send(msg))
    {
        qDebug("dbus send  call  failed\n");
        exit(-1);
    }

    SmsPlay *smsPlay = SmsPlay::instance();
    smsPlay->sms_play_set_stop();
}

void TipWindow::tipWindowPhoneDbusExtension(QString str)
{
    qDebug()<<"extension"<<str;
    QDBusMessage msg = QDBusMessage::createSignal("/", "com.asianux.pdaemon", "extension");
    msg<<str;
    if(!bus.send(msg))
    {
        qDebug("dbus send  call  failed\n");
        exit(-1);
    }

}

void TipWindow::tipWindowServerDbusDialBegin()
{
    qDebug()<<"begin";
    system("dbus-send / com.asianux.phone.begin ");
    netvideoWidget->hide();
    netmusicWidget->hide();
    neterrorWidget->hide();
    sertipWidget->hide();
    smsWidget->hide();
    sertipWidget->hide();
    dialWidget->hide();

    SmsPlay *smsPlay = SmsPlay::instance();
    smsPlay->sms_play_set_play();

    emit hide();

}

void TipWindow::tipWindowServerDbusDialEnd()
{
    qDebug()<<"end";
    system("dbus-send / com.asianux.phone.end ");
    netvideoWidget->hide();
    netmusicWidget->hide();
    neterrorWidget->hide();
    sertipWidget->hide();
    smsWidget->hide();
    sertipWidget->hide();
    dialWidget->hide();

    SmsPlay *smsPlay = SmsPlay::instance();
    smsPlay->sms_play_set_stop();

    emit hide();

}
