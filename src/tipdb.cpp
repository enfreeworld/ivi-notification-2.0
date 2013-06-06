#include "tipdb.h"

#define path "/usr/share/panda/theme/ivi-notification"

PhoneAddressDb::PhoneAddressDb()
{
    if(QSqlDatabase::contains("phoneAddress"))
        db = new QSqlDatabase(QSqlDatabase::database("phoneAddress"));
    else
        db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE","phoneAddress"));

    db->setDatabaseName(QString("%1/phoneAddress.db").arg(path));

    bool ok = db->open();

    if(ok)
    {
        qDebug()<<"address success";
        QString create_mobileAddress_table = "CREATE TABLE mobile_address (id INTEGER PRIMARY KEY, telAddress TEXT, telphone TEXT); " ;

        QSqlQuery mobile_query(*db);
        bool res = mobile_query.exec(create_mobileAddress_table);

        if(!res)
            qDebug()<<mobile_query.lastError();

        QString create_telephoneAddress_table = "CREATE TABLE telephone_address (id INTEGER PRIMARY KEY, telAddress TEXT, telphone TEXT); ";

        QSqlQuery telephone_query(*db);
        bool res2 = telephone_query.exec(create_telephoneAddress_table);

        if(!res2)
            qDebug()<<telephone_query.lastError();

    }
    else{
        qDebug()<<"address failed";
    }

}

PhoneAddressDb::~PhoneAddressDb()
{
    QString connection;
    connection=db->connectionName();
    if(!connection.isEmpty())
    {
        db->close();
        delete db;
        QSqlDatabase::removeDatabase(connection);
        qDebug()<<"----------------"<<connection;
    }
}

QString PhoneAddressDb::getPhoneAddress(QString str)
{
    if(str.left(1).compare("1") == 0) //1开头，来电为手机号
    {
        if(str.length() == 11)
        {
            QString number = str.left(7);

            QSqlQuery query(QString("select * from mobile_address where telphone = '%1' ;").arg(number), *db);

            if(query.first())
            {
                QString address = query.value(1).toString();

                return address;
            }
            else
            {
                return NULL;
            }
        }
    }
    else
    {
        QString number;

        if(str.left(1).compare("0") == 0) //来电为大陆固话
        {
            if(str.length() <= 11)    //直辖市或区号为三位数
            {
                number = str.left(3);
            }
            else if(str.length() == 12) //普通城市固话
            {
                number = str.left(4);
            }

            QSqlQuery query(QString("select * from telephone_address where telphone = '%1' ;").arg(number),*db);

            if(query.first())
            {
                QString address = query.value(1).toString();

                return address;
            }
            else
            {
                return NULL;
            }
        }
        else
            return NULL;
     }
}

ContactDb::ContactDb()
{
    if(QSqlDatabase::contains("contact"))
        db = new QSqlDatabase(QSqlDatabase::database("contact"));
    else
        db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", "contact"));

    db->setDatabaseName(QDir::homePath()+"/"+".contact.db");
    qDebug()<<QDir::homePath()+"/"+".contact.db";
    bool ok = db->open();
    if(ok)
    {
        qDebug()<<"contact success";
        QString create_contact_table="CREATE TABLE contact (id INTEGER PRIMARY KEY AUTOINCREMENT," "name TEXT," "mobile TEXT," "quanpin TEXT," "jianpin TEXT," "sort TEXT," "email TEXT);";

        QSqlQuery query(*db);
        bool  res=query.exec(create_contact_table);

    }
    else
    {
        qDebug()<<"contact failed";
    }


}

ContactDb::~ContactDb()
{
    QString connection;
    connection=db->connectionName();
    if(!connection.isEmpty())
    {
        db->close();
        delete db;
        QSqlDatabase::removeDatabase(connection);
        qDebug()<<"----------------"<<connection;
    }
}

QString ContactDb::getName(QString str)
{
    QSqlQuery query(QString("select name from contact where mobile=%1;").arg(str),*db);

    if(query.first())
    {
        QString name = query.value(0).toString();

        return name;
    }
    else
    {
        return NULL;
    }
}


SmsDb::SmsDb()
{
    if(QSqlDatabase::contains("sms"))
        db = new QSqlDatabase(QSqlDatabase::database("sms"));
    else
        db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", "sms"));

    db->setDatabaseName(QDir::homePath()+"/"+".sms.db");
    bool ok = db->open();
    if(ok)
    {
        qDebug()<<"sms success";
    }
    else
    {
        qDebug()<<"sms failed";
    }
}


SmsDb::~SmsDb()
{
    QString connection;
    connection=db->connectionName();
    db->close();
    delete db;
    QSqlDatabase::removeDatabase(connection);
    qDebug()<<"----------------"<<connection;
}

void SmsDb::smsDbUpdate(TipMessage *message)
{
    qDebug()<<"save";
    QSqlQuery query(QString(""),*db);

    query.exec(QString("update sms set READ=0 where NUMBER='%1' and TIME='%2'")
               .arg(message->number).arg(message->contenet3));

}


TelephoneDb::TelephoneDb()
{
    if(QSqlDatabase::contains("telephone"))
        db = new QSqlDatabase(QSqlDatabase::database("telephone"));
    else
        db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", "telephone"));

    db->setDatabaseName(QDir::homePath()+"/"+".call_history.db");
    bool ok = db->open();

    QString create_table_sql="CREATE TABLE call("
            "ROWID INTEGER PRIMARY KEY AUTOINCREMENT,"
            "number TEXT,"
            "date INTEGER,"
            "duration TEXT,"
            "status INTEGER,"
            "count INTEGER,"
            "misscallstate INTEGER);";

    if(ok)
    {
        QSqlQuery query(*db);
        bool  res=query.exec(create_table_sql);

        qDebug()<<"success";
    }
    else
    {
        qDebug()<<"failed";
    }
}

TelephoneDb::~TelephoneDb()
{
    QString connection;
    connection=db->connectionName();
    db->close();
    delete db;
    QSqlDatabase::removeDatabase(connection);
    qDebug()<<"----------------"<<connection;
}

void TelephoneDb::telephoneDbUpdate(QString str)
{
    QSqlQuery query(QString(""),*db);

    query.exec("select * from call order by date;");

    int i=0;

    while(query.next())
    {
        i++;
        //        qDebug()<<query.value(0).toString();
    }

    if(i >=100)
    {
        if(query.last())
        {
            qDebug()<<"------------"<<query.value(0).toString();
            QString id = query.value(0).toString();
            query.exec(QString("delete from call where ROWID = %1").arg(id));
        }
    }


    qDebug()<<"insert number is"<<str;
    query.exec(QString("select * from call where number= '%1' and status=%2 and misscallstate = 0;").arg(str).arg(MISSED));

    i=0;

    while(query.next())
    {
        i++;
    }

    qDebug()<<i;

    if(i ==0)
    {
        qDebug()<<"insert";
        QDateTime time = QDateTime::currentDateTime();
        query.exec(QString("insert into call values(NULL,'%1',\"%2\" ,'%3',%4,%5,0);")
                   .arg(str).arg(time.toTime_t())
                   .arg("00:00").arg(MISSED)
                   .arg(1));

    }
    else
    {
        query.first();
        int count = query.value(5).toInt();
        //QTime time  = QTime::currentTime();
        qDebug()<<"count"<<count;
        QDateTime time = QDateTime::currentDateTime();
        query.exec(QString("update call set date=\"%1\" ,duration='%2', count=%3  where number='%4' and status =%5 and misscallstate = 0;")
                   .arg(time.toTime_t()).arg("00:00")
                   .arg(count+1).arg(str)
                   .arg(MISSED));

    }


}
