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


#ifndef NEPOMUK_RESOURCESELECTIONDIALOG_H
#define NEPOMUK_RESOURCESELECTIONDIALOG_H

#include <KDialog>


namespace Nepomuk
{
    class Resource;
    namespace Query {
        class Query;
    }

    class ResourceSelectionDialog : public KDialog
    {
        Q_OBJECT

    public:
        ResourceSelectionDialog ( QWidget* parent = 0 );
        virtual ~ResourceSelectionDialog();

        void setText ( const QString& text );
        void setResources ( const QList<Resource>& resources );

        Resource selectedResource() const;

        static Resource selectResource ( QWidget* parent, const QString& text, const QList<Resource>& candidates, const QString& caption = QString() );
        static Resource selectResource ( QWidget* parent, const QString& text, const Query::Query& query, const QString& caption = QString() );

    private:
        class Private;
        Private* const d;

        Q_PRIVATE_SLOT ( d, void _k_selectionChanged() )
    };
}

#endif // NEPOMUK_RESOURCESELECTIONDIALOG_H
