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

#ifndef _NEPOMUK_RESOURCE_ACTIONS_H_
#define _NEPOMUK_RESOURCE_ACTIONS_H_

#include <QtCore/QObject>
#include <QtCore/QList>

#include "nepomukextras_export.h"

class QAction;
class KMenu;

namespace Nepomuk {

    class Resource;

    class NEPOMUKEXTRAS_EXPORT ResourceActions : public QObject
    {
        Q_OBJECT

    public:
        ResourceActions( QObject* parent = 0 );
        ~ResourceActions();

        QList<QAction*> resourceActions();

        void populateMenu( KMenu* menu );

    public Q_SLOTS:
        void setResource( const Nepomuk::Resource& res );
        void setResources( const QList<Nepomuk::Resource>& res );

    private:
        class Private;
        Private* const d;

        Q_PRIVATE_SLOT( d, void _k_deleteActionTriggered() )
    };
}

#endif
