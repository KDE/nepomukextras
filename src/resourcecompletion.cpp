/*
   Copyright (C) 2008-2010 by Sebastian Trueg <trueg at kde.org>

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

#include "resourcecompletion.h"

#include <Nepomuk/ResourceManager>
#include <Nepomuk/Resource>
#include <Nepomuk/Types/Class>
#include <Nepomuk/Query/AndTerm>
#include <Nepomuk/Query/OrTerm>
#include <Nepomuk/Query/LiteralTerm>
#include <Nepomuk/Query/ResourceTypeTerm>
#include <Nepomuk/Query/Query>

#include <Soprano/QueryResultIterator>
#include <Soprano/Model>
#include <Soprano/BindingSet>
#include <Soprano/Vocabulary/RDFS>
#include <Soprano/Vocabulary/RDF>

#include <kdebug.h>
#include <kicon.h>


class Nepomuk::ResourceCompletion::Private
{
public:
    Private()
        : type( Soprano::Vocabulary::RDFS::Resource() ) {
    }

    Types::Class type;
};


Nepomuk::ResourceCompletion::ResourceCompletion( QObject* parent )
    : KCompleter(),
      d( new Private() )
{
    setParent( parent );
}


Nepomuk::ResourceCompletion::~ResourceCompletion()
{
    delete d;
}


Nepomuk::Types::Class Nepomuk::ResourceCompletion::type() const
{
    return d->type;
}


void Nepomuk::ResourceCompletion::makeCompletion( const QString& string )
{
    kDebug() << string;

    if ( string.length() > 3 ) {
        Query::AndTerm term;
        term.addSubTerm( Query::LiteralTerm( string + '*' ) );
        if ( d->type.isValid() && d->type != Soprano::Vocabulary::RDFS::Resource() )
            term.addSubTerm( Query::ResourceTypeTerm( d->type ) );
        Query::Query query(term);
        query.setLimit( 10 );

        kDebug() << query.toSparqlQuery();

        Soprano::QueryResultIterator it
            = ResourceManager::instance()->mainModel()->executeQuery( query.toSparqlQuery(),
                                                                      Soprano::Query::QueryLanguageSparql );
        while ( it.next() ) {
            Resource res( it.binding( 0 ).uri() );
            double score = 1.0; //it[1].literal().toDouble();
            kDebug() << "Match for input" << string << res.uri();
            addCompletion( KCompletionItem( res.genericLabel(),
                                            QString( "%1 (%2)" ).arg( res.genericLabel() ).arg( Types::Class( res.type() ).label() ),
                                            res.genericDescription(),
                                            KIcon( res.genericIcon() ),
                                            score,
                                            res.resourceUri() ) );

        }
    }
}


void Nepomuk::ResourceCompletion::setType( const Nepomuk::Types::Class& type )
{
    d->type = type;
}


void Nepomuk::ResourceCompletion::itemActivated( const KCompletionItem& item )
{
    emit resourceActivated( item.userData().toUrl() );
}


void Nepomuk::ResourceCompletion::itemHighlighted( const KCompletionItem& item )
{
    emit resourceHighlighted( item.userData().toUrl() );
}

#include "resourcecompletion.moc"
