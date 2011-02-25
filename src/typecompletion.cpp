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

#include "typecompletion.h"
#include "pimo.h"

#include <Nepomuk/ResourceManager>
#include <Nepomuk/Types/Class>
#include <Nepomuk/Query/ComparisonTerm>
#include <Nepomuk/Query/LiteralTerm>
#include <Nepomuk/Query/AndTerm>
#include <Nepomuk/Query/OrTerm>
#include <Nepomuk/Query/ResourceTypeTerm>
#include <Nepomuk/Query/Query>

#include <Soprano/QueryResultIterator>
#include <Soprano/Model>
#include <Soprano/BindingSet>
#include <Soprano/RdfSchemaModel>
#include <Soprano/Vocabulary/RDF>
#include <Soprano/Vocabulary/RDFS>
#include <Soprano/Index/IndexFilterModel>

#include <kdebug.h>
#include <kicon.h>



class Nepomuk::TypeCompletion::Private
{
public:
    Types::Class baseType;
};


Nepomuk::TypeCompletion::TypeCompletion( QObject* parent )
    : KCompleter(),
      d( new Private() )
{
    setParent( parent );
}


Nepomuk::TypeCompletion::~TypeCompletion()
{
    delete d;
}


Nepomuk::Types::Class Nepomuk::TypeCompletion::baseType() const
{
    return d->baseType;
}


void Nepomuk::TypeCompletion::makeCompletion( const QString& string )
{
    // FIXME: if the string contains a : treat everything before it as a qname and match it to a namespace (for example by comparing the last section)

    kDebug() << string;

    if ( string.length() > 2 ) {
        Query::Query query(
            Query::AndTerm(
                Query::ResourceTypeTerm( Soprano::Vocabulary::RDFS::Class() ),
                Query::ComparisonTerm( Types::Property(), Query::LiteralTerm( string + '*' ) )
                )
            );

        Soprano::QueryResultIterator it
            = ResourceManager::instance()->mainModel()->executeQuery( query.toSparqlQuery(),
                                                                      Soprano::Query::QueryLanguageSparql );
        int cnt = 0;
        while ( it.next() ) {
            Types::Class type( it[0].uri() );
            double score = 1.0;// it[1].literal().toDouble();
            kDebug() << "possible match:" << type.label() << type.uri() << score;
            if ( ( !d->baseType.isValid() ||
                   type.isSubClassOf( d->baseType ) ) &&
                 score > 0.5 ) {
                kDebug() << "match:" << type.label() << type.uri();
                addCompletion( KCompletionItem( type.label(),
                                                type.label(),
                                                type.comment() + '\n' + '(' + type.uri().toString() + ')',
                                                type.icon(),
                                                score,
                                                type.uri() ) );

                if ( ++cnt >= 10 ) {
                    kDebug() << "Stopping at" << cnt << "results";
                    return;
                }
            }
        }
    }
}


void Nepomuk::TypeCompletion::setBaseType( const Nepomuk::Types::Class& type )
{
    d->baseType = type;
}


void Nepomuk::TypeCompletion::itemActivated( const KCompletionItem& item )
{
    emit typeActivated( item.userData().toUrl() );
}


void Nepomuk::TypeCompletion::itemHighlighted( const KCompletionItem& item )
{
    emit typeHighlighted( item.userData().toUrl() );
}

#include "typecompletion.moc"
