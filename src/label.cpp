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

#include <QPainter>
#include <QDebug>

#include "label.h"


Label::Label(QGraphicsItem *parent)
    : QGraphicsWidget(parent),
      m_fontColor(Qt::white),
      m_alignment(Qt::AlignLeft),
      m_elideMode(Qt::ElideRight)
{
    setFont(QFont("Nokia Sans",20));
    //this->setMaximumSize(200,68);
    //this->setMinimumSize(200,68);
}

QString Label::text() const
{
    return m_text;
}

void Label::setText(const QString &value)
{
    if (m_text != value) {
        m_text = value;
        update();

    }
}

QColor Label::fontColor() const
{
    return m_fontColor;
}

void Label::setFontColor(const QColor &color)
{
    if (m_fontColor != color) {
        m_fontColor = color;
        update();
    }
}

Qt::TextElideMode Label::elideMode() const
{
    return m_elideMode;
}

void Label::setElideMode(Qt::TextElideMode mode)
{
    if (m_elideMode != mode) {
        m_elideMode = mode;
        update();
    }
}

void Label::setAlignment(Qt::Alignment alignment)
{
    if (m_alignment != alignment) {
        m_alignment = alignment;
        update();
    }
}

void Label::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                  QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (m_text.isEmpty())
        return;

    const QRect &textRect = boundingRect().toRect();

    QFontMetrics metrics(font());
    const QString &elidedText = metrics.elidedText(m_text, m_elideMode,
                                                   textRect.width());

    painter->setFont(font());
    painter->setPen(m_fontColor);
    painter->drawText(textRect, Qt::TextSingleLine | m_alignment, elidedText);
}
