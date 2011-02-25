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

#include "strigitextextractionjob.h"

#include <QtCore/QThread>
#include <QtCore/QDataStream>
#include <QtCore/QFile>

#include <kurl.h>
#include <kdebug.h>

#include <strigi/bufferedstream.h>
#include <strigi/indexwriter.h>
#include <strigi/analysisresult.h>
#include <strigi/fieldtypes.h>
#include <strigi/fileinputstream.h>
#include <strigi/analyzerconfiguration.h>


namespace {
    /**
     * Strigi indexer which only extracts the plain text from the file.
     */
    class TextExtractor : public Strigi::IndexWriter
    {
    public:
        QString text() const { return m_text; }

        void startAnalysis( const Strigi::AnalysisResult*) {
            m_text.truncate(0);
        }

        void addText( const Strigi::AnalysisResult*, const char* s, int32_t n ) {
            m_text += QString::fromUtf8( s, n );
        }

        // all irrelevant
        void addValue(const Strigi::AnalysisResult*, const Strigi::RegisteredField*,
                      const std::string&) {}
        void addValue(const Strigi::AnalysisResult*, const Strigi::RegisteredField*,
                      const unsigned char*, uint32_t) {}
        void addValue(const Strigi::AnalysisResult*, const Strigi::RegisteredField*,
                      int32_t) {}
        void addValue(const Strigi::AnalysisResult*, const Strigi::RegisteredField*,
                      uint32_t) {}
        void addValue(const Strigi::AnalysisResult*, const Strigi::RegisteredField*,
                      double) {}
        void addValue(const Strigi::AnalysisResult*, const Strigi::RegisteredField*,
                      const std::string&, const std::string&) {}
        void addTriplet(const std::string&, const std::string&,
                        const std::string& ) {}
        void finishAnalysis(const Strigi::AnalysisResult*) {}
        void deleteEntries(const std::vector<std::string>&) {}
        void deleteAllEntries() {}

    private:
        QString m_text;
    };


    // taken from kdebase/runtime/nepomuk/services/strigi/indexscheduler.cpp
    class QDataStreamStrigiBufferedStream : public Strigi::BufferedStream<char>
    {
    public:
        QDataStreamStrigiBufferedStream( QDataStream& stream )
            : m_stream( stream ) {
        }

        int32_t fillBuffer( char* start, int32_t space ) {
            int r = m_stream.readRawData( start, space );
            if ( r == 0 ) {
                // Strigi's API is so weird!
                return -1;
            }
            else if ( r < 0 ) {
                // Again: weird API. m_status is a protected member of StreamBaseBase (yes, 2x Base)
                m_status = Strigi::Error;
                return -1;
            }
            else {
                return r;
            }
        }

    private:
        QDataStream& m_stream;
    };
}


class StrigiTextExtractionJob::Private : public QThread
{
public:
    KUrl m_url;
    QDataStream* m_stream;

    QString m_text;

    StrigiTextExtractionJob* q;

    void _k_finished() {
        q->emitResult();
    }

protected:
    void run() {
        // FIXME: implement fetching the data from any place via KIO
        Q_ASSERT( m_stream || m_url.isLocalFile() );

        // extract plain text
        TextExtractor extractor;
        Strigi::AnalyzerConfiguration config;
        Strigi::StreamAnalyzer analyzer( config );
        Strigi::AnalysisResult result( "fake", 0, extractor, analyzer );

        if ( m_stream ) {
            QDataStreamStrigiBufferedStream strigiStream( *m_stream );
            result.index( &strigiStream );
        }
        else {
            QFile file( m_url.toLocalFile() );
            file.open( QIODevice::ReadOnly );
            QDataStream stream( &file );
            QDataStreamStrigiBufferedStream strigiStream( stream );
            result.index( &strigiStream );
        }

        m_text = extractor.text();
    }
};


StrigiTextExtractionJob::StrigiTextExtractionJob( QObject* parent )
    : KJob( parent ),
      d( new Private() )
{
    d->m_stream = 0;
    d->q = this;
    connect( d, SIGNAL( finished() ),
             this, SLOT( _k_finished() ),
             Qt::QueuedConnection );
}


StrigiTextExtractionJob::~StrigiTextExtractionJob()
{
    delete d;
}


void StrigiTextExtractionJob::start()
{
    d->start();
}


QString StrigiTextExtractionJob::text() const
{
    return d->m_text;
}


void StrigiTextExtractionJob::setUrl( const KUrl& url )
{
    d->m_url = url;
}


void StrigiTextExtractionJob::setData( QDataStream* data )
{
    d->m_stream = data;
}

#include "strigitextextractionjob.moc"
