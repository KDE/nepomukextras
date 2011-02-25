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

#ifndef _KCOMPLETER_H_
#define _KCOMPLETER_H_

#include <QtGui/QCompleter>
#include <QtCore/QList>

#include "kcompletionitem.h"
#include "nepomukextras_export.h"

/**
 * Be aware that even when using QLineEdit::setCompletor setCompletionText
 * still ahs to be called manually (i.e. connected to the textChanged signal)
 *
 * FIXME: add more documentation
 *
 * \author Sebastian Trueg <trueg@kde.org>
 */
class NEPOMUKEXTRAS_EXPORT KCompleter : public QCompleter
{
    Q_OBJECT

public:
    KCompleter( QObject* parent = 0 );
    virtual ~KCompleter();

    QList<KCompletionItem> allItems() const;

Q_SIGNALS:
    void activated( const KCompletionItem& );
    void highlighted( const KCompletionItem& );

public Q_SLOTS:
    /**
     * Set the text to complete. This is independent
     * from QCompleter::setCompletionPrefix which
     * does not have to be called anymore.
     */
    void setCompletionText( const QString& );

protected:
    virtual void makeCompletion( const QString& ) = 0;

    /**
     * Called whenever an item is activated. Can be used to emit
     * custom activated signals or perform other operations. The
     * default implementation does nothing.
     */
    virtual void itemActivated( const KCompletionItem& item );

    /**
     * Called whenever an item is highlighted. Can be used to emit
     * custom activated signals or perform other operations. The
     * default implementation does nothing.
     */
    virtual void itemHighlighted( const KCompletionItem& item );

    void addCompletion( const KCompletionItem& item );

private:
    class Private;
    Private* const d;

    Q_PRIVATE_SLOT( d, void _k_activated( const QModelIndex& ) )
    Q_PRIVATE_SLOT( d, void _k_highlighted( const QModelIndex& ) )
};

#endif
