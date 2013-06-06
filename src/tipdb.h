#ifndef TIPDB_H
#define TIPDB_H
#include <QtSql>
#include <QDateTime>
#include "tipdata.h"

class PhoneAddressDb
{
public:
    PhoneAddressDb();
    ~PhoneAddressDb();

    QString getPhoneAddress(QString str);

private:
    QSqlDatabase *db;
};

class ContactDb
{
public:
    ContactDb();
    ~ContactDb();
    QString getName(QString str);
private:
    QSqlDatabase *db;
};


class SmsDb
{
public:
    SmsDb();
    ~SmsDb();
    void smsDbUpdate(TipMessage *message);
private:
    QSqlDatabase *db;
};

class TelephoneDb
{
public:
    TelephoneDb();
    ~TelephoneDb();
    void telephoneDbUpdate(QString str);
private:
    QSqlDatabase *db;
};

#endif // TIPDB_H
