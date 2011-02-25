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

#ifndef _NEPOMUK_MENU_H_
#define _NEPOMUK_MENU_H_

#include <KMenu>

#include "nepomukextras_export.h"

namespace Nepomuk {
    /**
     * Menu class which provides a few convinience methods for handling
     * Resources in combination with actions.
     */
    class NEPOMUKEXTRAS_EXPORT Menu : public KMenu
    {
        Q_OBJECT

    public:
        Menu( QWidget* parent = 0 );
        virtual ~Menu();

    protected:
        /**
         * Creates an action showing a busy spinner until deleted.
         */
        QAction* createBusyAction( QObject* parent = 0 ) const;

        /**
         * Set the action's text and tooltip. If the \p text is too long it will be
         * elided and the tooltip wil contain the full text.
         *
         * \param action The action to set the text on.
         * \param text The action text
         * \param typeLabel An optional resource type label which will always be shown (never be elided)
         */
        void setElidedActionText( QAction* action, const QString& text, const QString& typeLabel = QString() ) const;

    private:
        class Private;
        Private* const d;
    };
}

#endif
