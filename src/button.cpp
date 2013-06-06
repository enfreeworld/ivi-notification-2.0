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

#include <QPixmap>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>

#include "button.h"


class ButtonPrivate
{
public:
    ButtonPrivate(Button *qptr);

    void init();

    Button *q;
    QString text;
    bool isPressed;
    QPixmap normalPixmap;
    QPixmap pressedPixmap;
    QPixmap disabledPixmap;
};

ButtonPrivate::ButtonPrivate(Button *qptr)
    : q(qptr), isPressed(false)
{

}

void ButtonPrivate::init()
{
    q->setMinimumSize(normalPixmap.size());
    q->setMaximumSize(normalPixmap.size());
}


Button::Button(const QPixmap &normal, QGraphicsItem *parent)
    : QGraphicsWidget(parent),
      d(new ButtonPrivate(this))
{
    d->normalPixmap = normal;
    d->init();
}

Button::Button(const QPixmap &normal, const QPixmap &pressed,
               QGraphicsItem *parent)
    : QGraphicsWidget(parent),
      d(new ButtonPrivate(this))
{
    d->normalPixmap = normal;
    d->pressedPixmap = pressed;
    d->init();
}

Button::Button(const QPixmap &normal, const QPixmap &pressed,
               const QPixmap &disabled, QGraphicsItem *parent)
    : QGraphicsWidget(parent),
      d(new ButtonPrivate(this))
{
    d->normalPixmap = normal;
    d->pressedPixmap = pressed;
    d->disabledPixmap = disabled;
    d->init();
}

Button::~Button()
{
    delete d;
}

void Button::setPixmap(State state, const QPixmap &pixmap)
{
    bool dirty = false;

    if (state == NormalState) {
        d->normalPixmap = pixmap;
        dirty = (!d->isPressed && isEnabled());
        setMinimumSize(d->normalPixmap.size());
        setMaximumSize(d->normalPixmap.size());
    } else if (state == PressedState) {
        dirty = d->isPressed;
        d->pressedPixmap = pixmap;
    } else if (state == DisabledState) {
        dirty = !isEnabled();
        d->disabledPixmap = pixmap;
    }

    if (dirty)
        update();
}

QString Button::text() const
{
    return d->text;
}

void Button::setText(const QString &value)
{
    if (d->text != value) {
        d->text = value;
        update();
    }
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        d->isPressed = true;
        update();
        emit pressed();
    }
}

void Button::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        const bool isClick = d->isPressed;

        d->isPressed = false;
        update();
        emit released();

        if (isClick && contains(e->pos()))
            emit clicked();
    }
}

void Button::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget)
{
    Q_UNUSED(widget);

    if (!(option->state & QStyle::State_Enabled)) {
        const bool support = !d->disabledPixmap.isNull();
        painter->drawPixmap(0, 0, support ? d->disabledPixmap : d->normalPixmap);
    } else if (d->isPressed && !d->pressedPixmap.isNull())
        painter->drawPixmap(0, 0, d->pressedPixmap);
    else if (!d->normalPixmap.isNull())
        painter->drawPixmap(0, 0, d->normalPixmap);


    if (!d->text.isEmpty()) {
        const QRect &textRect = boundingRect().toRect();

        QFontMetrics metrics(font());
        const QString &elidedText = metrics.elidedText(d->text, Qt::ElideRight,
                                                       textRect.width());

        painter->setFont(font());
        painter->setPen(Qt::white);
        painter->drawText(textRect, Qt::TextSingleLine | Qt::AlignCenter, elidedText);
    }
}
