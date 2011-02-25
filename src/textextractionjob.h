/*
   This file is part of the Nepomuk KDE project.
   Copyright (C) 2011 Sebastian Trueg <trueg@kde.org>

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

#ifndef TEXTEXTRACTIONJOB_H
#define TEXTEXTRACTIONJOB_H

#include <KJob>

#include "nepomukextras_export.h"

namespace Nepomuk {
class Resource;

/**
 * The TextExtractionJob does its best to extract some plain text
 * from a resource. It first checks the resource's metadata, then
 * falls back to starting Strigi or even Olena for images.
 *
 * \author Sebastian Trueg <trueg@kde.org>
 */
class NEPOMUKEXTRAS_EXPORT TextExtractionJob : public KJob
{
    Q_OBJECT

public:
    TextExtractionJob(QObject *parent = 0);
    ~TextExtractionJob();

    /**
     * Get the extracted text. Call this method in the slot
     * connected to the KJob::result signal.
     */
    QString text() const;

public Q_SLOTS:
    void start();
    void setResource(const Nepomuk::Resource& res);

private:
    class Private;
    Private* const d;

    Q_PRIVATE_SLOT(d, void _k_strigiResult(KJob*))
    Q_PRIVATE_SLOT(d, void _k_mimetypeJobResult(KJob*))
    Q_PRIVATE_SLOT(d, void _k_olenaResult(KJob*))
};

} // namespace Nepomuk

#endif // TEXTEXTRACTIONJOB_H
