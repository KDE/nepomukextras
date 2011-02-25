/*
   Copyright (c) 2009 Sebastian Trueg <trueg@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "kcompletiondelegate.h"
#include "kcompletionitem.h"
#include "kcompletionmodel.h"

#include <QtGui/QPushButton>
#include <QtGui/QTextDocument>
#include <QtGui/QPainter>
#include <QtGui/QToolButton>

#include <kicon.h>


Q_DECLARE_METATYPE( KCompletionItem )

KCompletionDelegate::KCompletionDelegate( QObject* parent )
    : QStyledItemDelegate( parent )
{
}


KCompletionDelegate::~KCompletionDelegate()
{
}


namespace {
    QString itemText( const KCompletionItem& item )
    {
        return QString( "<p>%1<br/><i>%2</i>" )
            .arg( item.text() )
            .arg( item.toolTip() );
    }
}

void KCompletionDelegate::paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
    KCompletionItem item = index.data( KCompletionModel::CompletionItemRole ).value<KCompletionItem>();

    QTextDocument doc;
    doc.setHtml( QString("<html><body><div style=\"color:%1\">%2</div></body></html>")
                     .arg( option.state & QStyle::State_Selected
                           ? option.palette.color( QPalette::HighlightedText ).name()
                           : option.palette.color( QPalette::Text ).name() )
                 .arg( itemText( item ) ) );

    painter->save();

    if ( option.state & QStyle::State_Selected ) {
        painter->fillRect( option.rect, option.palette.highlight() );
    }

    painter->translate( option.rect.topLeft() );
    doc.drawContents( painter );

    painter->restore();
}


QSize KCompletionDelegate::sizeHint( const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
    Q_UNUSED( option );

    KCompletionItem item = index.data( KCompletionModel::CompletionItemRole ).value<KCompletionItem>();

    QTextDocument doc;
    doc.setHtml( itemText( item ) );
    return doc.size().toSize();
}

#include "kcompletiondelegate.moc"
