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

#ifndef _DATE_RANGE_H_
#define _DATE_RANGE_H_

#include <QtCore/QDate>
#include <nepomukextras_export.h>

class NEPOMUKEXTRAS_EXPORT DateRange
{
public:
    DateRange( const QDate& s = QDate(),
               const QDate& e = QDate() );
    ~DateRange();

    QDate start() const;
    QDate end() const;

    /**
     * Checks if both start and end are valid dates
     * and if end is after start.
     */
    bool isValid() const;

    /**
     * \returns a DateRange with both start and end
     * dates set to QDate::currentDate()
     */
    static DateRange today();

    /**
     * Takes KLocale::weekStartDay() into account.
     */
    static DateRange thisWeek();

    /**
     * Takes KLocale::weekStartDay() into account.
     */
    static DateRange weekOf( const QDate& date );

    static DateRange thisMonth();
    static DateRange monthOf( const QDate& date );

private:
    QDate m_start;
    QDate m_end;
};

bool operator==( const DateRange& r1, const DateRange& r2 );

#endif
