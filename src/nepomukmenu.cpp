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

#include "nepomukmenu.h"

#include <QtGui/QWidgetAction>

#include <KPixmapSequenceOverlayPainter>


class Nepomuk::Menu::Private
{
};


Nepomuk::Menu::Menu( QWidget* parent )
    : KMenu( parent ),
      d( 0 )
{
}


Nepomuk::Menu::~Menu()
{
    delete d;
}


QAction* Nepomuk::Menu::createBusyAction( QObject* parent ) const
{
    QWidget* w = new QWidget();
    w->setMinimumSize( 22, 22 );
    w->setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::Minimum );
    KPixmapSequenceOverlayPainter* busyPainter = new KPixmapSequenceOverlayPainter( w );
    busyPainter->setWidget( w );
    busyPainter->start();
	QWidgetAction* busyAction = new QWidgetAction( parent );
    busyAction->setDefaultWidget( w );
    return busyAction;
}


void Nepomuk::Menu::setElidedActionText( QAction* action, const QString& text, const QString& typeLabel ) const
{
    int maxWidth = fontMetrics().width( "xxxxxxxxxxxxxxxxxxxxxxxxxxxx" );
    QString typeSuffix;
    if ( !typeLabel.isEmpty() ) {
        typeSuffix = QString::fromLatin1( " (%1)" ).arg( typeLabel );
        maxWidth -= fontMetrics().width( typeSuffix );
    }

    // FIXME: use a more appropriate width value
    QString elidedText = fontMetrics().elidedText( text, Qt::ElideRight, maxWidth );
    if ( elidedText != text ) {
        action->setToolTip( text + typeSuffix );
    }
    action->setText( elidedText + typeSuffix );
}

#include "nepomukmenu.moc"
