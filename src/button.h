/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: qt-info@nokia.com
**
** This software, including documentation, is protected by copyright
** controlled by Nokia Corporation.  You may use this software in
** accordance with the terms and conditions contained in the Qt Phone
** Demo License Agreement.
**
****************************************************************************/

#ifndef BUTTON_H
#define BUTTON_H

#include <QGraphicsWidget>

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE


class ButtonPrivate;

class Button : public QGraphicsWidget
{
    Q_OBJECT

public:
    enum State { NormalState, PressedState, DisabledState };

    Button(const QPixmap &normal, QGraphicsItem *parent = 0);

    Button(const QPixmap &normal, const QPixmap &pressed,
           QGraphicsItem *parent = 0);

    Button(const QPixmap &normal, const QPixmap &pressed,
           const QPixmap &disabled, QGraphicsItem *parent = 0);

    ~Button();

    QString text() const;
    void setText(const QString &value);

    void setPixmap(State state, const QPixmap &pixmap);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0);

signals:
    void clicked();
    void pressed();
    void released();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);

private:
    ButtonPrivate *d;
};

#endif
