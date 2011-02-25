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

#include "kcompleter.h"
#include "kcompletionmodel.h"
#include "kcompletiondelegate.h"

#include <QtGui/QSortFilterProxyModel>
#include <QtGui/QAbstractItemView>

Q_DECLARE_METATYPE( KCompletionItem )


class KCompleter::Private
{
public:
    void _k_activated( const QModelIndex& index );
    void _k_highlighted( const QModelIndex& index );

    KCompletionModel* model;

    KCompleter* q;
};


void KCompleter::Private::_k_activated( const QModelIndex& index )
{
    KCompletionItem item = index.data( KCompletionModel::CompletionItemRole ).value<KCompletionItem>();
    q->itemActivated( item );
    emit q->activated( item );
}


void KCompleter::Private::_k_highlighted( const QModelIndex& index )
{
    KCompletionItem item = index.data( KCompletionModel::CompletionItemRole ).value<KCompletionItem>();
    q->itemHighlighted( item );
    emit q->highlighted( item );
}


KCompleter::KCompleter( QObject* parent )
    : QCompleter( parent ),
      d( new Private() )
{
    d->q = this;
    d->model = new KCompletionModel( this );
    QSortFilterProxyModel* proxyModel = new QSortFilterProxyModel( this );
    proxyModel->setSourceModel( d->model );
    proxyModel->setSortRole( KCompletionModel::InvertedScoreRole );
    proxyModel->setDynamicSortFilter( true );
    setModel( proxyModel );

    popup()->setItemDelegateForColumn( 0, new KCompletionDelegate( this ) );

    connect( this, SIGNAL( activated( QModelIndex ) ),
             this, SLOT( _k_activated( QModelIndex ) ) );
    connect( this, SIGNAL( highlighted( QModelIndex ) ),
             this, SLOT( _k_highlighted( QModelIndex ) ) );
}


KCompleter::~KCompleter()
{
    delete d;
}


void KCompleter::setCompletionText( const QString& text )
{
    d->model->clear();
    makeCompletion( text );
    setCompletionPrefix( text );
}


QList<KCompletionItem> KCompleter::allItems() const
{
    return d->model->allItems();
}


void KCompleter::addCompletion( const KCompletionItem& item )
{
    d->model->addCompletionItem( item );
}


void KCompleter::itemActivated( const KCompletionItem& )
{
    // do nothing
}


void KCompleter::itemHighlighted( const KCompletionItem& )
{
    // do nothing
}

#include "kcompleter.moc"
