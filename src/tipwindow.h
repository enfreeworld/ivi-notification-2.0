#ifndef TIPWINDOW_H
#define TIPWINDOW_H

#include <QtGui>
#include <QtCore>
#include <QtDBus>

#include "dialwidget.h"
#include "smswidget.h"
#include "netvideowidget.h"
#include "netmusicwidget.h"
#include "neterrorwidget.h"
#include "sertipwidget.h"
#include "timer.h"
#include "tipdata.h"
#include "tipdb.h"
#include "smsplay.h"
#include "filepath.h"

class TipWindow :public QGraphicsView
{
    Q_OBJECT
public:
    TipWindow(QWidget *parent=0);
    void tipWindowUpdateReal(TipMessage *message);
    MessageType tipWindowMessageType(QString str);
    void tipWindowUpdateDial(TipMessage *message);
    void tipWindowUpdateSms(TipMessage *message);
    void tipWindowUpdateNetmusic(TipMessage *message);
    void tipWindowUpdateNetvideo(TipMessage *message);
    void tipWindowUpdateNeterror(TipMessage *message);
    void tipWindowUpdateSertip(TipMessage *message);
    void tipWindowBgUpdate();
    ~TipWindow();

    enum VIEW_STATE{DIAL_VIEW,SMS_VIEW,MUSIC_VIEW,VIDEO_VIEW};
    VIEW_STATE state;

private slots:
    /*循环检查是否有提示*/
    void tipWindowCheckMessage();
    /*洗发提示的dbus消息*/
    void tipWindowDbusNotify(QString str1, QString str2,QString str3,QString str4);
    void tipWindowDbusClose();
    /*按下panel的tel键*/
    void tipWindowDbusPanel();
    /*按下panel的icall*/
    void tipWindowDbusIcall();
    /*按下panel的ecall键*/
    void tipWindowDbusEcall();
    /*按下panel的挂机键*/
    void tipWindowDbusHook();
    /*提供的拨号接口*/
    void tipWindowDbusSmscall(QString str);

    /*提供的发短信接口*/
    void tipWindowDbusSmssend(QString str);


    /*通话结束，提示窗口可以显示*/
    void tipWindowDbusTipshow();
    /*正在通话，提示窗口须隐藏*/
    void tipWindowDbusTiphide();
    void tipWindowWidgetNovisible();

    /*以下消息主要是在电话与服务端之间通信，起管理作用*/
    void tipWindowPhoneDbusCall(QString str);   //拨打电话
    void tipWindowPhoneDbusHook();  //挂断电话
    void tipWindowPhoneDbusAnswer();    //接听电话
    void tipWindowPhoneDbusReject();    //拒接电话
    void tipWindowPhoneDbusExtension(QString str);  //拨打分机号

    /*接受从服务端的反馈消息*/
    void tipWindowServerDbusDialBegin();    //开始通话
    void tipWindowServerDbusDialEnd();  //通话结束
signals:
    void show();
    void hide();
private:
    DialWidget *dialWidget;
    SmsWidget *smsWidget;
    NetvideoWidget *netvideoWidget;
    NetmusicWidget *netmusicWidget;
    NeterrorWidget *neterrorWidget;
    SertipWidget *sertipWidget;
    Timer *timer;
    TipMessage *videoMessage;
    bool newVideo;
    TipMessage *musicMessage;
    bool newMusic;
    TipMessage *smsMessage;
    bool newSms;
    TipMessage *errorMessage;
    bool newError;
    TipMessage *sertipMessgae;
    bool newSertip;
    MessageType curType;
    ContactDb *contactDb;
    PhoneAddressDb *phoneAddressDb;
    bool calling;
    bool windowVisable;
    static  QDBusConnection  bus;
};

#endif // TIPWINDOW_H
