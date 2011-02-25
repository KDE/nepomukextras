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

#include "resourceactions.h"

#include <nepomuk/resource.h>

#include <KIcon>
#include <KLocale>
#include <KAction>
#include <KMenu>


class Nepomuk::ResourceActions::Private
{
public:
    Private()
        : m_deleteAction( 0 ) {
    }

    void _k_deleteActionTriggered();

    QList<Resource> m_resources;

    QAction* m_deleteAction;
};


void Nepomuk::ResourceActions::Private::_k_deleteActionTriggered()
{
    // FIXME: ask for confirmation
    for ( int i = 0; i < m_resources.count(); ++i )
        m_resources[i].remove();
}


Nepomuk::ResourceActions::ResourceActions( QObject* parent )
    : QObject( parent ),
      d( new Private() )
{
}


Nepomuk::ResourceActions::~ResourceActions()
{
    delete d;
}


QList<QAction*> Nepomuk::ResourceActions::resourceActions()
{
    QList<QAction*> actions;

    if ( !d->m_deleteAction ) {
        d->m_deleteAction = new QAction( this );
        d->m_deleteAction->setText( i18nc( "@action", "Delete Resource" ) );
        d->m_deleteAction->setIcon( KIcon( "edit-delete" ) );
        connect( d->m_deleteAction, SIGNAL( triggered() ),
                 this, SLOT( _k_deleteActionTriggered() ) );
    }

    actions << d->m_deleteAction;

    return actions;
}


void Nepomuk::ResourceActions::populateMenu( KMenu* menu )
{
    Q_FOREACH( QAction* action, resourceActions() ) {
        menu->addAction( action );
    }
}


void Nepomuk::ResourceActions::setResource( const Nepomuk::Resource& res )
{
    setResources( QList<Resource>() << res );
}


void Nepomuk::ResourceActions::setResources( const QList<Nepomuk::Resource>& res )
{
    d->m_resources = res;
}

#include "resourceactions.moc"
