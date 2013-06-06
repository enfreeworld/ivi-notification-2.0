#ifndef TIPDATA_H
#define TIPDATA_H

#include <QtGui>
#include <QtCore>

/*数据类*/

typedef enum
{
    TIP_SRC_DIAL = 0,
    TIP_SRC_SMS_COMMON,    // 短信， 考虑在系统栏上添加提示， 可以直接调出短信程序
    TIP_SRC_SMS_FRIREQ,    // 请求好友位置短信
    TIP_SRC_SMS_FRIREC,    // 回复好友位置短信
    TIP_SRC_SMS_NETMUSIC, //在线音乐
    TIP_SRC_SMS_NETMOVIE, //在线电影
    TIP_SRC_SMS_SERTIP,  //欠费提醒
    TIP_SRC_SMS_NETERROR, //网络错误
    /*
    TIP_SRC_SMS_PUBLIC,    // 短信方式， 交通公告, 暂时不考虑短信方式
    TIP_SRC_SMS_BRULES,    // 短信方式， 违章提示, 暂时不考虑短信方式
    */
    TIP_SRC_NONE
}MessageType;

typedef enum
{
    SHOW_DIAL,
    SHOW_SMS,
    SHOW_MUSIC,
    SHOW_MOVIE,
    SHOW_SERTIP,
    SHOW_NETERROR,
    SHOW_NONE
}ShowType;

typedef enum
{
    SMS_DB,
    CONTACT_DB,
    TELEPHONE_DB,
    NONE_DB
}DbType;

/*消息所带参数*/

typedef struct
{
    QString type;
    QString number;
    QString content1;
    QString content2;
    QString contenet3;

}TipMessage;

typedef enum
{
    MISSED,
    IN,
    OUT,
    NONE
}CallFlags;

class TipData
{
public:
    TipData();
    ~TipData();
};

#endif // TIPDATA_H
