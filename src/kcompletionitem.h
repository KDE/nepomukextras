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

#ifndef _KCOMPLETION_ITEM_H_
#define _KCOMPLETION_ITEM_H_

#include <QtCore/QSharedDataPointer>
#include <QtCore/QVariant>

#include "nepomukextras_export.h"

class QString;
class QIcon;

class NEPOMUKEXTRAS_EXPORT KCompletionItem
{
public:
    KCompletionItem();
    KCompletionItem( const KCompletionItem& );
    KCompletionItem( const QString& completionText, // FIXME: we do not need the completionText anymore
                     const QString& text,
                     const QString& desc,
                     const QIcon& icon,
                     double score = 1.0,
                     const QVariant& userData = QVariant() );
    ~KCompletionItem();

    KCompletionItem& operator=( const KCompletionItem& );

    QString completionText() const;
    QString text() const;
    QString toolTip() const;
    QIcon icon() const;
    double score() const;
    QVariant userData() const;

private:
    class Private;
    QSharedDataPointer<Private> d;
};

#endif
