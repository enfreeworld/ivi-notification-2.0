#ifndef DIALWIDGET_H
#define DIALWIDGET_H

#include <QtGui>
#include <QtCore>
#include "button.h"
#include "label.h"
#include "tipdata.h"
#include "tipdb.h"
#include "smsplay.h"

class DialWidget : public QGraphicsWidget
{
    Q_OBJECT
public:
    DialWidget();
    ~DialWidget();
    void dialWidgetUpdate(TipMessage *message);
    bool telephoneIsRunning();
    void dialWidgetEcall();
    void dialWidgetIcall();
    void dialWidgetSmscall(QString str);
    void dialWidgetSetName(QString str);
    void dialWidgetSetAddress(QString str);
public slots:
    void dialRightButtonClick();
    void dialLeftButtonClick();
    void dialWidgetThreadFinished();
signals:
    void noVisible();
private:
    Button *rightButton;
    Button *leftButton;
    Label *name;
    Label *number;
    Label *phoneAddress;
    QGraphicsPixmapItem *bg;
    QString phoneNumber;
    QString ecallNumber;
    QString icallNumber;
    TelephoneDb *telephoneDb;

    bool is_calling;
};

#endif // DIALWIDGET_H
