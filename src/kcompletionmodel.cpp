/*
   Copyright (C) 2008 by Sebastian Trueg <trueg at kde.org>

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

#include "kcompletionmodel.h"
#include "kcompleter.h"

#include <QtGui/QIcon>

#include <kdebug.h>


Q_DECLARE_METATYPE( KCompletionItem )

class KCompletionModel::Private
{
public:
    Private( KCompletionModel* parent )
        : q( parent ) {
    }

    QList<KCompletionItem> items;

private:
    KCompletionModel* q;
};


KCompletionModel::KCompletionModel( QObject* parent )
    : QAbstractItemModel( parent ),
      d( new Private( this ) )
{
}


KCompletionModel::~KCompletionModel()
{
    delete d;
}


void KCompletionModel::clear()
{
    d->items.clear();
    reset();
}


QList<KCompletionItem> KCompletionModel::allItems() const
{
    return d->items;
}


void KCompletionModel::addCompletionItem( const KCompletionItem& item )
{
    beginInsertRows( QModelIndex(), d->items.count(), d->items.count() );
    d->items.append( item );
    endInsertRows();
}


QVariant KCompletionModel::data( const QModelIndex& index, int role ) const
{
    if ( index.isValid() &&
         index.row() < d->items.count() &&
         index.column() == 0 ) {

        KCompletionItem item = d->items[index.row()];

        switch( role ) {
        case Qt::EditRole:
            // we disable the QCompleter comparision here by simply always returning the entered text
            return static_cast<KCompleter*>( QObject::parent() )->completionPrefix();

        case Qt::DisplayRole:
            return item.text();

        case Qt::ToolTipRole:
            return item.toolTip();

        case Qt::DecorationRole:
            return item.icon();

        case UserDataRole:
            return item.userData();

        case CompletionItemRole:
            return QVariant::fromValue( item );

        case ScoreRole:
            return item.score();

        case InvertedScoreRole:
            return ( 1.0 - item.score() );

        default:
            return QVariant();
        }
    }

    return QVariant();
}


int KCompletionModel::columnCount( const QModelIndex& ) const
{
    return 1;
}


int KCompletionModel::rowCount( const QModelIndex& parent ) const
{
    if ( parent.isValid() ) {
        return 0;
    }
    else {
        return d->items.count();
    }
}


QModelIndex KCompletionModel::index( int row, int column, const QModelIndex& ) const
{
    if ( row < d->items.count() && column == 0 ) {
        return createIndex( row, column );
    }
    return QModelIndex();
}


QModelIndex KCompletionModel::parent( const QModelIndex& ) const
{
    return QModelIndex();
}

#include "kcompletionmodel.moc"
