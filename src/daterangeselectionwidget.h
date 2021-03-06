/*
   Copyright (c) 2009 Sebastian Trueg <trueg@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License or (at your option) version 3 or any later version
   accepted by the membership of KDE e.V. (or its successor approved
   by the membership of KDE e.V.), which shall act as a proxy
   defined in Section 14 of version 3 of the license.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef _DATE_RANGE_SELECTION_WIDGET_H_
#define _DATE_RANGE_SELECTION_WIDGET_H_

#include <QtGui/QWidget>
#include <QtCore/QDate>

#include <nepomukextras_export.h>

class DateRange;
class QAction;

class NEPOMUKEXTRAS_EXPORT DateRangeSelectionWidget : public QWidget
{
    Q_OBJECT

public:
    DateRangeSelectionWidget( QWidget* parent = 0 );
    ~DateRangeSelectionWidget();

    /**
     * One of:
     * \li a range between two valid dates
     * \li only a valid end date: all dates before that date
     * \li only a valid start date: all dates after that date
     * \li invalid range: anytime
     */
    DateRange range() const;

public Q_SLOTS:
    void setRange( const DateRange& range );

Q_SIGNALS:
    void rangeChanged( const DateRange& range );

private:
    class Private;
    Private* const d;

    Q_PRIVATE_SLOT( d, void _k_anytimeClicked() )
    Q_PRIVATE_SLOT( d, void _k_beforeClicked() )
    Q_PRIVATE_SLOT( d, void _k_afterClicked() )
    Q_PRIVATE_SLOT( d, void _k_rangeClicked() )
    Q_PRIVATE_SLOT( d, void _k_rangeClicked( QAction* ) )
    Q_PRIVATE_SLOT( d, void _k_rangeStartEdited( const QDate& date ) )
    Q_PRIVATE_SLOT( d, void _k_rangeEndEdited( const QDate& date ) )
    Q_PRIVATE_SLOT( d, void _k_calendarDateClicked( const QDate& date ) )
};

#endif
