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

#include "daterange.h"

#include <KGlobal>
#include <KLocale>
#include <KCalendarSystem>


DateRange::DateRange( const QDate& s,
                      const QDate& e )
    : m_start( s ),
      m_end( e )
{
}


DateRange::~DateRange()
{
}


QDate DateRange::start() const
{
    return m_start;
}


QDate DateRange::end() const
{
    return m_end;
}


bool DateRange::isValid() const
{
    return m_start.isValid() && m_end.isValid() && m_start <= m_end;
}


// static
DateRange DateRange::today()
{
    return DateRange( QDate::currentDate(), QDate::currentDate() );
}


namespace {
    /**
     * Put \p day into the week range 1...7
     */
    int dateModulo( int day ) {
        day = day%7;
        if ( day == 0 )
            return 7;
        else
            return day;
    }
}

// static
DateRange DateRange::thisWeek()
{
    return weekOf( QDate::currentDate() );
}


// static
DateRange DateRange::weekOf( const QDate& date )
{
    const int weekStartDay = KGlobal::locale()->weekStartDay();
    const int weekEndDay = dateModulo( weekStartDay+6 );

    DateRange range;

    if ( weekStartDay > date.dayOfWeek() )
        range.m_start = date.addDays( - (date.dayOfWeek() + 7 - weekStartDay) );
    else
        range.m_start = date.addDays( - (date.dayOfWeek() - weekStartDay) );

    if ( weekEndDay < date.dayOfWeek() )
        range.m_end = date.addDays( weekEndDay + 7 - date.dayOfWeek() );
    else
        range.m_end = date.addDays( weekEndDay - date.dayOfWeek() );

    return range;
}


// static
DateRange DateRange::thisMonth()
{
    return monthOf( QDate::currentDate() );
}


// static
DateRange DateRange::monthOf( const QDate& date )
{
    return DateRange( QDate( date.year(), date.month(), 1 ),
                      QDate( date.year(), date.month(), KGlobal::locale()->calendar()->daysInMonth( date ) ) );
}


bool operator==( const DateRange& r1, const DateRange& r2 )
{
    return r1.start() == r2.start() && r1.end() == r2.end();
}
