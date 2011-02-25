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

#ifndef _STRIGI_TEXT_EXTRACTION_JOB_H_
#define _STRIGI_TEXT_EXTRACTION_JOB_H_

#include <kjob.h>

class QDataStream;
class KUrl;

/**
 * \brief Extracts plain text from data streams
 *
 * StrigiTextExtractionJob uses Strigi to extract the plain text from documents.
 * Strigi supports a variaty of mimetypes including plain text, html, pdf, odt.
 *
 * This job wraps Strigi in a separate thread to not block the gui while analyzing
 * the data.
 *
 * \author Sebastian Trueg <trueg@kde.org>
 */
class StrigiTextExtractionJob : public KJob
{
    Q_OBJECT

public:
    StrigiTextExtractionJob( QObject* parent = 0 );
    ~StrigiTextExtractionJob();

    /**
     * Get the extracted text. Call this method in the slot
     * connected to the KJob::result signal.
     */
    QString text() const;

public Q_SLOTS:
    void start();
    void setUrl( const KUrl& url );
    void setData( QDataStream* data );

private:
    class Private;
    Private* const d;

    Q_PRIVATE_SLOT( d, void _k_finished() )
};

#endif
