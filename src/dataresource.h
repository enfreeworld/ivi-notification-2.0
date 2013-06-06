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

#ifndef RESOURCE_H
#define RESOURCE_H

#include <QPixmap>
#include <QVariant>


class QSettings;


class Resource
{
public:
    static void setIniFile(const QString &fileName);

    static QPixmap pixmap(const QString &path);
    static void setPixmapPrefix(const QString &prefix);

    static bool containsValue(const QString &key);

    static QVariant value(const QString &key, const QVariant &value = QVariant());

    static int intValue(const QString &key, int value = 0);
    static double doubleValue(const QString &key, double value = 0);
    static QString stringValue(const QString &key, const QString &value = QString());

private:
    Resource();
    ~Resource();
    static Resource *instance();

    QSettings *settings;
    QString pixmapPrefix;
};

#endif
