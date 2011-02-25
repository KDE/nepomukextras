/*
   Copyright (C) 2008-2009 by Sebastian Trueg <trueg at kde.org>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "kcompletionitem.h"

#include <QtCore/QSharedData>
#include <QtCore/QVariant>
#include <QtGui/QIcon>


class KCompletionItem::Private : public QSharedData
{
public:
    Private()
        : score( 0.0 ) {
    }

    QString completionText;
    QString text;
    QString toolTip;
    QIcon icon;
    double score;
    QVariant userData;
};


KCompletionItem::KCompletionItem()
    : d( new Private() )
{
}

KCompletionItem::KCompletionItem( const KCompletionItem& other )
    : d( other.d )
{
}


KCompletionItem::KCompletionItem( const QString& cText,
                                  const QString& text,
                                  const QString& desc,
                                  const QIcon& icon,
                                  double score,
                                  const QVariant& userData )
    : d( new Private() )
{
    d->completionText = cText;
    d->text = text;
    d->toolTip = desc;
    d->icon = icon;
    d->score = score;
    d->userData = userData;
}


KCompletionItem::~KCompletionItem()
{
}


KCompletionItem& KCompletionItem::operator=( const KCompletionItem& other )
{
    d = other.d;
    return *this;
}


QString KCompletionItem::completionText() const
{
    return d->completionText;
}


QString KCompletionItem::text() const
{
    return d->text;
}


QString KCompletionItem::toolTip() const
{
    return d->toolTip;
}


QIcon KCompletionItem::icon() const
{
    return d->icon;
}


double KCompletionItem::score() const
{
    return d->score;
}


QVariant KCompletionItem::userData() const
{
    return d->userData;
}
