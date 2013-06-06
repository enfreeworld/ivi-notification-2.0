#include <QtGui>
#include <QTextCodec>
#include <gst/gst.h>
#include "widget.h"
#include "dataresource.h"
#include "filepath.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

   // Q_INIT_RESOURCE(ivi_notification);
    gst_init(&argc,&argv);

    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);

    Resource::setIniFile("/etc/ivi-notification/ivi-notification.ini");

    QTranslator translator;

    qDebug()<<QLocale::system().name();

    if (QLocale::system().name() == "zh_CN")
    {
        qDebug() << "load zh translator file ";
        translator.load("notify_zh",IMAGE_PATH);
        QApplication::installTranslator(&translator);
    }

    Widget w;

    return a.exec();
}

