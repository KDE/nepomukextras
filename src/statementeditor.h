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

#ifndef _NEPOMUK_STATEMENT_EDITOR_H_
#define _NEPOMUK_STATEMENT_EDITOR_H_

#include <QtGui/QWidget>

#include "nepomukextras_export.h"

namespace Nepomuk {
    class Variant;
    class Resource;
    namespace Types {
        class Property;
        class Class;
    }

    class NEPOMUKEXTRAS_EXPORT StatementEditor : public QWidget
    {
        Q_OBJECT

    public:
        StatementEditor( QWidget* parent = 0 );
        ~StatementEditor();

    Q_SIGNALS:
        void statementCreated( const Nepomuk::Types::Property& prop, const Nepomuk::Variant& value );

    public Q_SLOTS:
        void setResource( const Nepomuk::Resource& res );

    private:
        bool eventFilter( QObject* watched, QEvent* event );

        class Private;
        Private* const d;

        Q_PRIVATE_SLOT( d, void _k_propertyActivated( const Nepomuk::Types::Property& ) )
        Q_PRIVATE_SLOT( d, void _k_returnPressed() )
        Q_PRIVATE_SLOT( d, void _k_dateTimeEditingFinished() )
        Q_PRIVATE_SLOT( d, void _k_resourceActivated( const Nepomuk::Resource& ) )
        Q_PRIVATE_SLOT( d, void _k_typeActivated( const Nepomuk::Types::Class& ) )
    };
}

#endif
