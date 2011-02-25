/*
   Copyright (C) 2008 by Sebastian Trueg <trueg at kde.org>

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

#ifndef _NEPOMUK_RESOURCE_COMPLETION_H_
#define _NEPOMUK_RESOURCE_COMPLETION_H_

#include "kcompleter.h"
#include "nepomukextras_export.h"

namespace Nepomuk {

    class Resource;
    namespace Types {
        class Class;
    }

    class NEPOMUKEXTRAS_EXPORT ResourceCompletion : public KCompleter
    {
        Q_OBJECT

    public:
        ResourceCompletion( QObject* parent = 0 );
        ~ResourceCompletion();

        Types::Class type() const;

    Q_SIGNALS:
        void resourceActivated( const Nepomuk::Resource& resource );
        void resourceHighlighted( const Nepomuk::Resource& resource );

    public Q_SLOTS:
        void setType( const Nepomuk::Types::Class& type );

    protected:
        void makeCompletion( const QString& string );
        void itemActivated( const KCompletionItem& item );
        void itemHighlighted( const KCompletionItem& item );

    private:
        class Private;
        Private* const d;
    };
}

#endif
