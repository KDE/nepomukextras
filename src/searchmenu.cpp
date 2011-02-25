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

#include "searchmenu.h"

#include <QtCore/QList>
#include <QtCore/QPointer>
#include <QtGui/QWidgetAction>

#include <KLineEdit>
#include <KPixmapSequenceOverlayPainter>
#include <KLocale>
#include <KDebug>

#include <Nepomuk/Resource>
#include <Nepomuk/Types/Class>
#include <Nepomuk/Query/QueryServiceClient>
#include <Nepomuk/Query/AndTerm>
#include <Nepomuk/Query/Query>
#include <Nepomuk/Query/ComparisonTerm>
#include <Nepomuk/Query/LiteralTerm>

#include <Soprano/Vocabulary/NAO>

Q_DECLARE_METATYPE( Nepomuk::Query::Result )

class Nepomuk::SearchMenu::Private
{
public:
    Private()
        : m_configFlags( SearchMenu::NoFlags ) {
    }

    QAction* createResultAction( const Query::Result& result ) const;

    void _k_query();
    void _k_newEntries( const QList<Nepomuk::Query::Result>& results );
    void _k_finishedListing();
    void _k_resultActionTriggered();
    void _k_aboutToShow();

    Query::Term m_coreQueryTerm;
    SearchMenu::ConfigurationFlags m_configFlags;

    KLineEdit* m_lineEdit;
    QWidgetAction* m_editAction;
    QList<QAction*> m_resultActions;

    QPointer<QAction> m_busyAction;

    Query::QueryServiceClient m_queryClient;

    SearchMenu* q;
};


QAction* Nepomuk::SearchMenu::Private::createResultAction( const Query::Result& result ) const
{
    kDebug() << result.resource().resourceUri();
    QAction* action = new QAction( q );
    q->setElidedActionText( action, result.resource().genericLabel(), Types::Class( result.resource().resourceType() ).label() );
    action->setData( QVariant::fromValue( result ) );
    q->connect( action, SIGNAL( triggered() ), SLOT( _k_resultActionTriggered() ) );
    return action;
}


void Nepomuk::SearchMenu::Private::_k_query()
{
    // stop potentially old query
    m_queryClient.close();

    // remove all previous results
    qDeleteAll( m_resultActions );
    m_resultActions.clear();

    const QString queryString = m_lineEdit->text();

    if (!queryString.isEmpty() || m_coreQueryTerm.isValid()) {
        // create the busy action
        if ( !m_busyAction ) {
            m_busyAction = q->createBusyAction( q );
        }

        // construct query
//         Query::Query query = Query::QueryParser::parseQuery( queryString );
        Query::Query query( Query::ComparisonTerm( Soprano::Vocabulary::NAO::prefLabel(),
                                                   Query::LiteralTerm( queryString ),
                                                   Query::ComparisonTerm::Regexp ) );
        if ( m_coreQueryTerm.isValid() ) {
            query.setTerm( Query::AndTerm( query.term(), m_coreQueryTerm ) );
        }

        // TODO: add more... buttons which will then get the next 10 using offset
        query.setLimit( 10 );

        // start the new query
        m_queryClient.query( query );
    }
}


void Nepomuk::SearchMenu::Private::_k_newEntries( const QList<Nepomuk::Query::Result>& results )
{
    Q_FOREACH( const Query::Result& result, results ) {
        QAction* action = createResultAction( result );
        m_resultActions.append( action );
        q->insertAction( m_busyAction, action );
    }
}


void Nepomuk::SearchMenu::Private::_k_finishedListing()
{
    // no need for busy indicator anymore
    delete m_busyAction;

    // we do not handle updates so far
    m_queryClient.close();
}


void Nepomuk::SearchMenu::Private::_k_resultActionTriggered()
{
    Query::Result result = qobject_cast<QAction*>( q->sender() )->data().value<Query::Result>();
    emit q->resultTriggered( result );
}


void Nepomuk::SearchMenu::Private::_k_aboutToShow()
{
    kDebug();
    m_lineEdit->setFocus( Qt::OtherFocusReason );
}


Nepomuk::SearchMenu::SearchMenu( QWidget* parent )
    : Menu( parent ),
      d( new Private() )
{
    d->q = this;

    d->m_lineEdit = new KLineEdit();
    d->m_editAction = new QWidgetAction( this );
    d->m_editAction->setDefaultWidget( d->m_lineEdit );
    addAction( d->m_editAction );

    connect( &d->m_queryClient, SIGNAL( newEntries( QList<Nepomuk::Query::Result> ) ),
             this, SLOT( _k_newEntries( QList<Nepomuk::Query::Result> ) ) );
    connect( &d->m_queryClient, SIGNAL( finishedListing() ),
             this, SLOT( _k_finishedListing() ) );

    connect( this, SIGNAL( aboutToShow() ), this, SLOT( _k_aboutToShow() ) );

    setConfigurationFlags(SearchMenu::NoFlags);
}


Nepomuk::SearchMenu::~SearchMenu()
{
    delete d;
}


void Nepomuk::SearchMenu::setConfigurationFlags( ConfigurationFlags flags )
{    
    disconnect( d->m_lineEdit, SIGNAL( returnPressed() ),
                this, SLOT( _k_query() ) );
    disconnect( d->m_lineEdit, SIGNAL( textEdited( QString ) ),
                this, SLOT( _k_query() ) );

    if ( flags & SearchMenu::SearchWhileYouType ) {
        connect( d->m_lineEdit, SIGNAL( textEdited( QString ) ),
                 this, SLOT( _k_query() ) );
    }
    else {
        connect( d->m_lineEdit, SIGNAL( returnPressed() ),
                 this, SLOT( _k_query( ) ) );
    }

    d->m_configFlags = flags;
}


Nepomuk::SearchMenu::ConfigurationFlags Nepomuk::SearchMenu::configurationFlags() const
{
    return d->m_configFlags;
}


void Nepomuk::SearchMenu::setCoreQueryTerm( const Query::Term& term )
{
    d->m_coreQueryTerm = term;
    if( d->m_configFlags & SearchMenu::SearchOnSetCoreTerm ) {
        d->_k_query();
    }
}

#include "searchmenu.moc"
