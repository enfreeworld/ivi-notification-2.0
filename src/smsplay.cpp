#include "smsplay.h"

SmsPlay *SmsPlay::smsPlay = NULL;

static gboolean sms_play_bus_call(GstBus *bus,GstMessage *msg,gpointer data)
{
    switch (GST_MESSAGE_TYPE(msg))
    {
    case GST_MESSAGE_EOS:
    {
        SmsPlay *smsPlay = SmsPlay::instance();

        gst_element_set_state(GST_ELEMENT(smsPlay->pipeline),GST_STATE_NULL);
        gst_element_set_state(GST_ELEMENT(smsPlay->pipeline),GST_STATE_PLAYING);
        app_agent_set_stop(smsPlay->appAgent);

    }
        break;
    case GST_MESSAGE_ERROR:
    {
    }
        break;
    default:
        break;
    }
}


SmsPlay::SmsPlay()
{
    pipeline = NULL;
    src = NULL;
    bus = NULL;

    pcmPath = "file:///usr/share/panda/theme/ivi-notification/ring.mp3";
    pipeline = gst_element_factory_make("playbin","pipeline");
    g_object_set(pipeline,"uri",qPrintable(pcmPath),NULL);
    //gst_element_set_state(pipeline,GST_STATE_PLAYING);
    gst_element_set_state(pipeline,GST_STATE_NULL);
    bus = (GstElement *)gst_pipeline_get_bus(GST_PIPELINE(pipeline));
    gst_bus_add_watch((GstBus *)bus,sms_play_bus_call,this);


    appAgent = app_agent_new(AP_PHONE, "com.asianux.appphone", SmsPlay::absl_event_filter);

    if(app_agent_register(appAgent))
    {
        qDebug()<<"register success\n";
    }

}

SmsPlay::~SmsPlay()
{

}

SmsPlay *SmsPlay::instance()
{
    if(smsPlay == NULL)
    {
        smsPlay = new SmsPlay();
    }

    return smsPlay;
}

int SmsPlay::absl_event_filter(AppState p)
{
    AppState tmp = (AppState)(p);
    switch (tmp)
    {
    case ACTIVE:
    {
        SmsPlay *smsPlay = SmsPlay::instance();
        app_agent_set_play(smsPlay->appAgent);
        gst_element_set_state(GST_ELEMENT(smsPlay->pipeline),GST_STATE_PLAYING);
    }
        break;
    case SLEEP:
    {
        SmsPlay *smsPlay = SmsPlay::instance();

        gst_element_set_state(GST_ELEMENT(smsPlay->pipeline),GST_STATE_PAUSED);
        app_agent_set_stop(smsPlay->appAgent);
    }
        break;
    case DEAD:
    {
        SmsPlay *smsPlay = SmsPlay::instance();

        gst_element_set_state(GST_ELEMENT(smsPlay->pipeline),GST_STATE_NULL);
        app_agent_set_stop(smsPlay->appAgent);
    }
        break;
    default:
        break;
    }

    return 0;

}

void SmsPlay::sms_play_set_play(QString str)
{
    qDebug()<<str;

    gboolean play =  app_agent_check_can_play(appAgent);

    if(play)
    {
        qDebug()<<"**********************************";
        app_agent_set_play(appAgent);
        gst_element_set_state(pipeline,GST_STATE_PLAYING);
    }
}

void SmsPlay::sms_play_set_play()
{
    gboolean play =  app_agent_check_can_play(appAgent);

    if(play)
    {
        app_agent_set_play(appAgent);
    }
}

void SmsPlay::sms_play_set_stop()
{
    gst_element_set_state(GST_ELEMENT(smsPlay->pipeline),GST_STATE_NULL);
    app_agent_set_stop(appAgent);
}

void SmsPlay::sms_play_set_pause()
{

}


