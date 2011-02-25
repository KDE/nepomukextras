/*
   This file is part of the Nepomuk KDE project.
   Copyright (C) 2008-2010 Sebastian Trueg <trueg@kde.org>

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

#include "newresourcedialog.h"
#include "ui_newresourcedialog.h"
#include "resourceselectiondialog.h"

#include <nepomuk/resource.h>
#include <nepomuk/resourcemanager.h>
#include <nepomuk/class.h>

#include <Nepomuk/Query/Query>
#include <Nepomuk/Query/AndTerm>
#include <Nepomuk/Query/OrTerm>
#include <Nepomuk/Query/LiteralTerm>
#include <Nepomuk/Query/ResourceTypeTerm>
#include <Nepomuk/Query/ComparisonTerm>

#include <KDebug>
#include <KLocale>
#include <KTitleWidget>
#include <KMessageBox>
#include <KPixmapSequenceOverlayPainter>

#include <QtGui/QListWidget>
#include <QtGui/QListWidgetItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QLabel>

#include <Soprano/Node>
#include <Soprano/Util/AsyncQuery>
#include <Soprano/Vocabulary/NAO>


class Nepomuk::NewResourceDialog::Private : public Ui_NewResourceDialog
{
public:
    enum DialogResult {
        CreateNewResource = 8908987,
        ReuseExistingResource = 6876748,
        Canceled = 6857
    };

    void findExistingResources();

    void _k_nextExistingResourceReady( Soprano::Util::AsyncQuery* );
    void _k_existingResourcesQueryFinished( Soprano::Util::AsyncQuery* );

    KPixmapSequenceOverlayPainter m_overlayPainter;

    Types::Class m_resourceType;

    // a list of existing resources that might be duplicates
    // created via findExistingResources
    QList<Resource> m_existingResources;

    // the resource the user selected in the dialog
    Resource m_selectedResource;

    NewResourceDialog* q;
};


void Nepomuk::NewResourceDialog::Private::findExistingResources()
{
    m_overlayPainter.start();

    // check if the resource might already exist
    Query::Query query(
        Query::AndTerm(
            Query::ComparisonTerm( Soprano::Vocabulary::NAO::prefLabel(), Query::LiteralTerm( m_editLabel->text() ) ),
            Query::ResourceTypeTerm( m_resourceType )
        )
    );
    query.setLimit( 5 );

    Soprano::Util::AsyncQuery* sQuery = Soprano::Util::AsyncQuery::executeQuery( ResourceManager::instance()->mainModel(),
                                        query.toSparqlQuery(), Soprano::Query::QueryLanguageSparql );
    q->connect( sQuery, SIGNAL( nextReady( Soprano::Util::AsyncQuery* ) ),
                SLOT( _k_nextExistingResourceReady( Soprano::Util::AsyncQuery* ) ) );
    q->connect( sQuery, SIGNAL( finished( Soprano::Util::AsyncQuery* ) ),
                SLOT( _k_existingResourcesQueryFinished( Soprano::Util::AsyncQuery* ) ) );
}


void Nepomuk::NewResourceDialog::Private::_k_nextExistingResourceReady( Soprano::Util::AsyncQuery* query )
{
    m_existingResources.append( query->binding( 0 ).uri() );
    query->next();
}


void Nepomuk::NewResourceDialog::Private::_k_existingResourcesQueryFinished( Soprano::Util::AsyncQuery* )
{
//    m_overlayPainter.stop();

    if ( m_existingResources.isEmpty() ) {
        q->done( Private::CreateNewResource );
    }
    else {
        ResourceSelectionDialog dlg( q );
        dlg.setButtons( KDialog::Ok|KDialog::No|KDialog::Cancel );
        dlg.setButtonText( KDialog::Ok, i18nc( "@label:button", "Use existing" ) );
        dlg.setButtonText( KDialog::No, i18nc( "@label:button", "Create new" ) );
        dlg.setText( i18nc( "@info", "Existing resources matching your input have been found. "
                            "Do you want to use an existing resource or actually create a new one?" ) );
        dlg.setResources(m_existingResources);

        int r = dlg.exec();
        if ( r == KDialog::Ok ) {
            m_selectedResource = dlg.selectedResource();
            q->done( ReuseExistingResource );
        }
        else if ( r == KDialog::No ) {
            q->done( CreateNewResource );
        }
        else {
            q->done( Canceled );
        }
    }
}


Nepomuk::NewResourceDialog::NewResourceDialog( QWidget* parent )
        : KDialog( parent ),
        d( new Private() )
{
    d->q = this;

    setCaption( i18nc( "@title:window", "Create new Resource" ) );
    setButtons( Ok|Cancel );
    enableButtonOk( false );

    d->setupUi( mainWidget() );

    // place the spinner at the bottom left of the dialog
    d->m_overlayPainter.setWidget( this );
    d->m_overlayPainter.setAlignment( Qt::AlignLeft|Qt::AlignBottom );
    d->m_overlayPainter.setOffset( QPoint( KDialog::marginHint(), -KDialog::marginHint() ) );

    connect( d->m_editLabel, SIGNAL( textChanged(const QString&) ),
             this, SLOT( slotLabelChanged(const QString&) ) );

}


Nepomuk::NewResourceDialog::~NewResourceDialog()
{
    delete d;
}


QSize Nepomuk::NewResourceDialog::sizeHint() const
{
    return QSize(400, 256);
}

void Nepomuk::NewResourceDialog::slotLabelChanged( const QString& text )
{
    enableButtonOk( !text.isEmpty() );
}


void Nepomuk::NewResourceDialog::slotButtonClicked( int button )
{
    if ( button == KDialog::Ok ) {
        if ( d->m_editLabel->text().isEmpty() ) {
            KMessageBox::sorry( this, i18nc( "@info error message box",
                                             "Please specify a label for the new resource" ),
                                i18nc( "@title:window Message box title",
                                       "No label specified" ) );
        }
        else if ( d->m_editLabel->text().length() < 4 ) {
            KMessageBox::sorry( this, i18nc( "@info error message box",
                                             "The label should contain at least four characters" ),
                                i18nc( "@title:window Message box title",
                                       "Invalid label specified" ) );
        }
        else {
            enableButton(KDialog::Ok, false);
            // TODO: show a spinner somewhere
            d->findExistingResources();
        }
    }
    else {
        KDialog::slotButtonClicked( button );
    }
}


Nepomuk::Resource Nepomuk::NewResourceDialog::createResource( const Types::Class& type, QWidget* parent )
{
    NewResourceDialog dlg( parent );
    dlg.d->m_resourceType = type;
    dlg.d->m_labelTitle->setText( i18nc( "@title:window", "Create New %1", type.label() ) );
    dlg.d->m_labelTitle->setComment( i18nc( "@title:window subtitle to previous message", "with optional icon and description" ) );
    dlg.d->m_labelTitle->setPixmap( KIcon( "nepomuk" ).pixmap( 32, 32 ) );

    dlg.d->m_editLabel->setFocus();

    switch ( dlg.exec() ) {
    case Private::CreateNewResource: {
        QString name = dlg.d->m_editLabel->text();
        QString comment = dlg.d->m_editComment->text();
        QString icon = dlg.d->m_buttonIcon->icon();

        Nepomuk::Resource newResource( QUrl(), type.uri() );
        newResource.setLabel( name );
        newResource.addIdentifier( name );
        if ( !comment.isEmpty() ) {
            newResource.setDescription( comment );
        }
        if ( !icon.isEmpty() ) {
            newResource.addSymbol( icon );
        }
        return newResource;
    }

    case Private::ReuseExistingResource:
        return dlg.d->m_selectedResource;

    default:
        return Nepomuk::Resource();
    }
}

#include "newresourcedialog.moc"
