#include "widget.h"

#include <QX11Info>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

Widget::Widget(QWidget*p, Qt::WindowFlags f)
    : QWidget(p,f)
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint );

    //setFixedSize(800,480);
    resize(800,480);

    this->setWindowFlags(Qt::X11BypassWindowManagerHint);
   // this->setWindowFlags(Qt::WindowStaysOnTopHint);

    setWindowTitle("ivi-notification");
    setAttribute(Qt::WA_TranslucentBackground);
    this->move(0,0);

    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(255,0,0,200));
    pal.setColor(QPalette::Window, QColor(255,255,0,200));
    setPalette(pal);

    tipWindow = new TipWindow(this);
    connect(tipWindow,SIGNAL(show()),this,SLOT(widgetShowWindow()));
    connect(tipWindow,SIGNAL(hide()),this,SLOT(widgetHideWindow()));

    QDBusConnection::sessionBus().registerService("com.asianux.notification");

    QDBusConnection::sessionBus().connect(QString(),QString(),
                                          "com.asianux.notification","showwindow",
                                          this,SLOT(widgetShowWindow()));

    /*QDBusConnection::sessionBus().connect(QString(),QString(),
                                          "com.asianux.notification","hidewindow",
                                          this,SLOT(widgetHideWindow()));*/
    this->hide();
}

Widget::~Widget()
{
    delete tipWindow;
}

void Widget::widgetShowWindow()
{
    /*
    this->show();
    system("dbus-send / com.nokia.monkeyking.homeforbit &");
    this->activateWindow();
    this->showFullScreen();
    qDebug()<<"showwindow";*/

    this->move(0,0);
    this->showNormal();
    this->activateWindow();

    /*this->hide();
    this->setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    this->show();
    */

    XClientMessageEvent event;

    memset( &event, '\0', sizeof( XClientMessageEvent ) );

    event.type = ClientMessage;
    Display *display =QX11Info::display();
    event.message_type = XInternAtom( display, "_NET_ACTIVE_WINDOW", False );
    event.display = display;
    event.window = DefaultRootWindow(display);
    event.format = 32;
    event.data.l[ 0 ] = 1;
    event.data.l[ 1 ] = 0;
    event.data.l[ 2 ] = 0;
    event.data.l[ 3 ] = 0;
    event.data.l[ 4 ] = 0;

    XSendEvent( display, DefaultRootWindow(display), False,
              SubstructureRedirectMask, (XEvent*)&event );
}

void Widget::closeEvent(QCloseEvent *e)
{
    e->ignore();
}

void Widget::widgetHideWindow()
{
    this->hide();
    system("dbus-send / com.nokia.monkeyking.homeallow &");
    qDebug()<<"hideWindow";
}
