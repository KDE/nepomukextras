/*
   Copyright (C) 2009 by Sebastian Trueg <trueg at kde.org>

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

#ifndef _NEPOMUK_PROPERTY_COMPLETION_H_
#define _NEPOMUK_PROPERTY_COMPLETION_H_

#include "kcompleter.h"
#include "nepomukextras_export.h"

namespace Nepomuk {
    namespace Types {
        class Class;
        class Literal;
        class Property;
    }

    class NEPOMUKEXTRAS_EXPORT PropertyCompleter : public KCompleter
    {
        Q_OBJECT

    public:
        PropertyCompleter( QObject* parent = 0 );
        ~PropertyCompleter();

    Q_SIGNALS:
        void propertyActivated( const Nepomuk::Types::Property& property );
        void propertyHighlighted( const Nepomuk::Types::Property& property );

    public Q_SLOTS:
        /**
         * By default the domain is not defined and ignored.
         */
        void setDomain( const Nepomuk::Types::Class& domain );

        /**
         * By default the range is not defined and ignored.
         */
        void setRange( const Nepomuk::Types::Class& range );
        void setRange( const Nepomuk::Types::Literal& range );

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
