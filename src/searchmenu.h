/*
   This file is part of the Nepomuk KDE project.
   Copyright (C) 2010 Sebastian Trueg <trueg@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) version 3, or any
   later version accepted by the membership of KDE e.V. (or its
   successor approved by the membership of KDE e.V.), which shall
   act as a proxy defined in Section 6 of version 3 of the license.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _NEPOMUK_SEARCH_MENU_H_
#define _NEPOMUK_SEARCH_MENU_H_

#include "nepomukmenu.h"

#include <Nepomuk/Query/Result>

#include <QtCore/QList>

#include "nepomukextras_export.h"

namespace Nepomuk {
    namespace Query {
        class Term;
    }

    /**
     * A menu which provides a search bar at the top and will list the results
     * as menu items. When a result is selected the resultTriggered() signal
     * is emitted.
     */
    class NEPOMUKEXTRAS_EXPORT SearchMenu : public Menu
    {
        Q_OBJECT

    public:
        SearchMenu( QWidget* parent = 0 );
        ~SearchMenu();

        enum ConfigurationFlag {
            NoFlags = 0x0,
            /**
             * Indicates to search on typing
             */
            SearchWhileYouType = 0x1,
            /**
             * Indicates to search when the core Term is set
             */
            SearchOnSetCoreTerm = 0x2
        };
        Q_DECLARE_FLAGS( ConfigurationFlags, ConfigurationFlag )

        void setConfigurationFlags( ConfigurationFlags flags );
        ConfigurationFlags configurationFlags() const;

        /**
         * Set a basic term to always restrict the query.
         * By default this is an invalid Term, meaning results
         * are only determined by the user input.
         */
        void setCoreQueryTerm( const Query::Term& term );

    Q_SIGNALS:
        void resultTriggered( const Nepomuk::Query::Result& result );

    private:
        class Private;
        Private* const d;

        Q_PRIVATE_SLOT( d, void _k_query() )
        Q_PRIVATE_SLOT( d, void _k_newEntries( const QList<Nepomuk::Query::Result>& results ) )
        Q_PRIVATE_SLOT( d, void _k_finishedListing() )
        Q_PRIVATE_SLOT( d, void _k_resultActionTriggered() )
        Q_PRIVATE_SLOT( d, void _k_aboutToShow() )
    };
}

Q_DECLARE_OPERATORS_FOR_FLAGS( Nepomuk::SearchMenu::ConfigurationFlags )

#endif
