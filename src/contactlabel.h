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

#ifndef CONTACTLABEL_H
#define CONTACTLABEL_H

#include <QGraphicsWidget>


class ContactLabel : public QGraphicsWidget
{
    Q_OBJECT

public:
    ContactLabel(QGraphicsItem *parent = 0);

    QString text() const;
    void setText(const QString &value);

    QColor fontColor() const;
    void setFontColor(const QColor &color);

    Qt::TextElideMode elideMode() const;
    void setElideMode(Qt::TextElideMode mode);

    void setAlignment(Qt::Alignment alignment);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0);

private:
    QString m_text;
    QColor m_fontColor;
    Qt::Alignment m_alignment;
    Qt::TextElideMode m_elideMode;
};

#endif
