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

#include <QSettings>

#include "dataresource.h"


Resource::Resource()
    : settings(new QSettings()),
      pixmapPrefix(":/")
{

}

Resource::~Resource()
{
    delete settings;
}

Resource *Resource::instance()
{
    static Resource result;
    return &result;
}

void Resource::setIniFile(const QString &fileName)
{
    Resource *d = instance();

    if (d->settings)
        delete d->settings;

    d->settings = new QSettings(fileName, QSettings::IniFormat);
}

QPixmap Resource::pixmap(const QString &path)
{
    Resource *d = instance();
    return QPixmap(QString("%1%2").arg(d->pixmapPrefix, path));
}

void Resource::setPixmapPrefix(const QString &prefix)
{
    Resource *d = instance();
    d->pixmapPrefix = prefix;
}

bool Resource::containsValue(const QString &key)
{
    Resource *d = instance();
    return d->settings->contains(key);
}

QVariant Resource::value(const QString &key, const QVariant &value)
{
    Resource *d = instance();

    if (d->settings->contains(key))
        return d->settings->value(key, value);
    else {
        qWarning("Resource: key '%s' not found", key.toLatin1().data());
        return QVariant();
    }
}

int Resource::intValue(const QString &key, int value)
{
    return Resource::value(key, value).toInt();
}

double Resource::doubleValue(const QString &key, double value)
{
    return Resource::value(key, value).toDouble();
}

QString Resource::stringValue(const QString &key, const QString &value)
{
    return Resource::value(key, value).toString();
}
