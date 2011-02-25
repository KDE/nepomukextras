/*
   Copyright (C) 2009-2010 by Sebastian Trueg <trueg at kde.org>

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

#include "propertycompleter.h"

#include <Nepomuk/ResourceManager>
#include <Nepomuk/Resource>
#include <Nepomuk/Types/Class>
#include <Nepomuk/Types/Property>
#include <Nepomuk/Types/Literal>
#include <Nepomuk/Query/QueryParser>

#include <Soprano/QueryResultIterator>
#include <Soprano/Model>
#include <Soprano/BindingSet>
#include <Soprano/Vocabulary/RDFS>
#include <Soprano/Vocabulary/RDF>
#include <Soprano/Index/IndexFilterModel>

#include <kdebug.h>
#include <kicon.h>


class Nepomuk::PropertyCompleter::Private
{
public:
    Types::Class domain;
    Types::Class range;
    Types::Literal literalRange;
};


Nepomuk::PropertyCompleter::PropertyCompleter( QObject* parent )
    : KCompleter( parent ),
      d( new Private() )
{
}


Nepomuk::PropertyCompleter::~PropertyCompleter()
{
    delete d;
}


void Nepomuk::PropertyCompleter::setDomain( const Nepomuk::Types::Class& domain )
{
    d->domain = domain;
}


void Nepomuk::PropertyCompleter::setRange( const Nepomuk::Types::Class& range )
{
    d->range = range;
    d->literalRange = Types::Literal();
}


void Nepomuk::PropertyCompleter::setRange( const Nepomuk::Types::Literal& range )
{
    d->literalRange = range;
    d->range = Types::Class();
}


void Nepomuk::PropertyCompleter::makeCompletion( const QString& string )
{
    kDebug() << string;

    if ( string.length() > 2 ) {
        Query::QueryParser parser;
        QList<Types::Property> properties = parser.matchProperty( string );

        int cnt = 0;
        Q_FOREACH( const Types::Property& property, properties ) {
            if ( !d->domain.isValid() ||
                 property.domain() == d->domain ||
                 d->domain.isSubClassOf( property.domain() ) ) {
                double score = 1.0; //it[1].literal().toDouble();
                kDebug() << "Match for input" << string << property.uri() << score;
                addCompletion( KCompletionItem( property.label(),
                                                property.label(),
                                                property.comment(),
                                                KIcon( property.icon() ),
                                                score,
                                                property.uri() ) );

                if ( ++cnt >= 10 ) {
                    kDebug() << "Stopping at" << cnt << "results";
                    return;
                }
            }
        }
    }
}


void Nepomuk::PropertyCompleter::itemActivated( const KCompletionItem& item )
{
    emit propertyActivated( item.userData().toUrl() );
}


void Nepomuk::PropertyCompleter::itemHighlighted( const KCompletionItem& item )
{
    emit propertyHighlighted( item.userData().toUrl() );
}

#include "propertycompleter.moc"

