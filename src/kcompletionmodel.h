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

#ifndef _KCOMPLETION_MODEL_H_
#define _KCOMPLETION_MODEL_H_

#include <QtCore/QAbstractItemModel>

class KCompleter;
class KCompletionItem;

/**
 * Internal class
 */
class KCompletionModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    KCompletionModel( QObject* parent = 0 );
    ~KCompletionModel();

    void clear();
    void addCompletionItem( const KCompletionItem& item );

    QList<KCompletionItem> allItems() const;

    enum Roles {
        CompletionItemRole = 7777,
        ScoreRole = 678589,
        InvertedScoreRole = 68798, /**< used for sorting */
        UserDataRole = 7778
    };

    QVariant data( const QModelIndex& index, int role ) const;
    int columnCount( const QModelIndex& parent = QModelIndex() ) const;
    int rowCount( const QModelIndex& parent = QModelIndex() ) const;
    QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
    QModelIndex parent( const QModelIndex& index ) const;

private:
    class Private;
    Private* const d;
};

#endif
