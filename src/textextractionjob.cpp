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

#include "nepomukextras_config.h"

#include "textextractionjob.h"
#include "strigitextextractionjob.h"
#ifdef HAVE_KOLENA
#include <kolena/olenatextextractionjob.h>
#endif

#include <Nepomuk/Vocabulary/PIMO>
#include <Nepomuk/Vocabulary/NIE>
#include <Nepomuk/File>
#include <Nepomuk/Variant>

#include <KIO/MimetypeJob>
#include <KIO/Job>

class Nepomuk::TextExtractionJob::Private
{
public:
    Resource m_resource;
    QString m_text;

    void _k_strigiResult(KJob* job);
    void _k_mimetypeJobResult( KJob* );
    void _k_olenaResult(KJob*);

    TextExtractionJob* q;
};

void Nepomuk::TextExtractionJob::Private::_k_strigiResult(KJob* job)
{
    if(!job->error()) {
        m_text = static_cast<StrigiTextExtractionJob*>(job)->text();
    }

#ifdef HAVE_KOLENA
    if(m_text.isEmpty()) {
        // last try: Olena, but only or image files
        // we use KIO::MimetypeJob instead of Strigi values to be more generic and also be able to handle
        // non-analyzed files
        const KUrl url = m_resource.toFile().url();
        KIO::MimetypeJob* job = KIO::mimetype( url, KIO::HideProgressInfo );
        connect( job, SIGNAL( result( KJob* ) ), q, SLOT( _k_mimetypeJobResult( KJob* ) ) );
    }
    else {
        // nothing to be done
        q->emitResult();
    }
#else
    q->emitResult();
#endif
}


void Nepomuk::TextExtractionJob::Private::_k_mimetypeJobResult( KJob* job )
{
#ifdef HAVE_KOLENA
    KIO::MimetypeJob* mtjob = static_cast<KIO::MimetypeJob*>( job );
    if ( mtjob->mimetype().contains("image") ) {
        OlenaTextExtractionJob* olenaJob = OlenaTextExtractionJob::extractText( mtjob->url() );
        connect(olenaJob, SIGNAL(result(KJob*)),
                q, SLOT(_k_olenaResult(KJob*)));
        olenaJob->start();
    }
    else {
        q->emitResult();
    }
#else
    Q_UNUSED(job);
#endif
}

void Nepomuk::TextExtractionJob::Private::_k_olenaResult(KJob* job)
{
#ifdef HAVE_KOLENA
    OlenaTextExtractionJob* olenaJob = static_cast<OlenaTextExtractionJob*>(job);
    m_text = olenaJob->text();
    q->emitResult();
#else
    Q_UNUSED(job);
#endif
}


Nepomuk::TextExtractionJob::TextExtractionJob(QObject *parent) :
    KJob(parent),
    d(new Private())
{
    d->q = this;
}

Nepomuk::TextExtractionJob::~TextExtractionJob()
{
    delete d;
}

QString Nepomuk::TextExtractionJob::text() const
{
    return d->m_text;
}

void Nepomuk::TextExtractionJob::start()
{
    d->m_text = d->m_resource.property( Nepomuk::Vocabulary::NIE::plainTextContent() ).toString();
    if ( d->m_text.isEmpty() ) {
        if ( d->m_resource.hasType( Nepomuk::Vocabulary::PIMO::Thing() ) &&
             d->m_resource.hasProperty( Nepomuk::Vocabulary::PIMO::groundingOccurrence() ) ) {
            d->m_text = d->m_resource.property( Nepomuk::Vocabulary::PIMO::groundingOccurrence() )
                    .toResource()
                    .property( Nepomuk::Vocabulary::NIE::plainTextContent() )
                    .toString();
        }
    }

    if(d->m_text.isEmpty() && d->m_resource.isFile()) {
        // run Strigi on the file
        Nepomuk::File file = d->m_resource.toFile();
        StrigiTextExtractionJob* strigiJob = new StrigiTextExtractionJob(this);
        strigiJob->setUrl(file.url());
        connect(strigiJob, SIGNAL(result(KJob*)),
                this, SLOT(_k_strigiResult(KJob*)));
        strigiJob->start();
    }
    else {
        emitResult();
    }
}

void Nepomuk::TextExtractionJob::setResource(const Nepomuk::Resource &res)
{
    d->m_resource = res;
}

#include "textextractionjob.moc"
