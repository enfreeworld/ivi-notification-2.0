#ifndef SMSPLAY_H
#define SMSPLAY_H

#include <app-agent.h>
#include <gst/gst.h>
#include <glib.h>
#include <QDir>
#include <QString>
#include <QDebug>


class SmsPlay
{
public:
    static SmsPlay *instance();
    static SmsPlay *smsPlay;
    static int absl_event_filter(AppState p);
    SmsPlay();
    ~SmsPlay();
    GstElement *pipeline;
    GstElement *src;
    GstElement *bus;
    AppAgent *appAgent;
    QString pcmPath;

    void sms_play_set_play(QString str);
    void sms_play_set_play();
    void sms_play_set_stop();
    void sms_play_set_pause();

};

#endif // SMSPLAY_H
