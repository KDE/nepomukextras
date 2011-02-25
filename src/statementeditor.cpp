/*
   Copyright (C) 2009 by Sebastian Trueg <trueg at kde.org>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "statementeditor.h"
#include "propertycompleter.h"
#include "resourcecompletion.h"
#include "typecompletion.h"

#include <klineedit.h>
#include <kdebug.h>

#include <QtGui/QLabel>
#include <QtGui/QHBoxLayout>
#include <QtGui/QKeyEvent>
#include <QtGui/QDateTimeEdit>

#include <Nepomuk/Resource>
#include <Nepomuk/Variant>
#include <Nepomuk/Types/Literal>
#include <Nepomuk/Types/Class>
#include <Nepomuk/Types/Property>

#include <Soprano/Vocabulary/XMLSchema>
#include <Soprano/Vocabulary/RDFS>

namespace {
    enum EditorState {
        EditingProperty,
        EditingValue
    };
}


class Nepomuk::StatementEditor::Private
{
public:
    Private()
        : m_lineEdit( 0 ),
          m_dateTimeEdit( 0 ),
          m_state( EditingProperty ) {
    }

    StatementEditor* q;

    QLabel* m_propertyLabel;
    KLineEdit* m_lineEdit;
    QDateTimeEdit* m_dateTimeEdit;
    EditorState m_state;

    Resource m_resource;

    PropertyCompleter* m_propertyCompleter;
    ResourceCompletion* m_resourceCompleter;
    TypeCompletion* m_typeCompleter;

    Types::Property m_selectedProperty;

    void setState( EditorState );
    void _k_propertyActivated( const Nepomuk::Types::Property& prop );
    void _k_returnPressed();
    void _k_dateTimeEditingFinished();
    void _k_resourceActivated( const Nepomuk::Resource& res );
    void _k_typeActivated( const Nepomuk::Types::Class& );
};


void Nepomuk::StatementEditor::Private::setState( EditorState state )
{
    // reset connections for simplicity
    m_resourceCompleter->disconnect( q );
    m_propertyCompleter->disconnect( q );
    m_typeCompleter->disconnect( q );
    m_lineEdit->disconnect( m_resourceCompleter );
    m_lineEdit->disconnect( m_propertyCompleter );
    m_lineEdit->disconnect( m_typeCompleter );
    q->disconnect( m_lineEdit );
    q->disconnect( m_dateTimeEdit );
    m_lineEdit->clear();

    switch( state ) {
    case EditingProperty:
        m_propertyLabel->hide();
        m_dateTimeEdit->hide();
        m_lineEdit->show();
        m_lineEdit->setCompleter( m_propertyCompleter );
        m_lineEdit->setClickMessage( i18n( "Type in property..." ) );
        q->connect( m_propertyCompleter, SIGNAL(propertyActivated(Nepomuk::Types::Property)),
                    SLOT(_k_propertyActivated(Nepomuk::Types::Property)) );
        m_propertyCompleter->connect( m_lineEdit, SIGNAL(textEdited(QString)),
                                      SLOT(setCompletionText(QString)) );
        if ( m_resource.isValid() )
            m_propertyCompleter->setDomain( m_resource.resourceType() );
        else
            m_propertyCompleter->setDomain( Types::Class() );
        break;

    case EditingValue:
        m_propertyLabel->setText( m_selectedProperty.label() );
        m_propertyLabel->show();
        if ( m_selectedProperty.literalRangeType().isValid() ) {
            m_lineEdit->setCompleter( 0 );
            if ( m_selectedProperty.literalRangeType().dataTypeUri() == Soprano::Vocabulary::XMLSchema::dateTime() ) {
                m_lineEdit->hide();
                m_dateTimeEdit->show();
                q->connect( m_dateTimeEdit, SIGNAL(editingFinished()),
                            SLOT(_k_dateTimeEditingFinished()) );
            }
            else {
                m_lineEdit->show();
                m_dateTimeEdit->hide();
                q->connect( m_lineEdit, SIGNAL(returnPressed()),
                            SLOT(_k_returnPressed()) );
            }
        }
        else if ( m_selectedProperty.range().uri() == Soprano::Vocabulary::RDFS::Class() ) {
            m_lineEdit->setCompleter( m_typeCompleter );
            q->connect( m_typeCompleter, SIGNAL(typeActivated(Nepomuk::Types::Class)),
                        SLOT(_k_typeActivated(Nepomuk::Types::Class)) );
            m_typeCompleter->connect( m_lineEdit, SIGNAL(textEdited(QString)),
                                      SLOT(setCompletionText(QString)) );
        }
        else {
            m_lineEdit->setCompleter( m_resourceCompleter );
            m_resourceCompleter->setType( m_selectedProperty.range() );
            q->connect( m_resourceCompleter, SIGNAL(resourceActivated(Nepomuk::Resource)),
                        SLOT(_k_resourceActivated(Nepomuk::Resource)) );
            m_resourceCompleter->connect( m_lineEdit, SIGNAL(textEdited(QString)),
                                          SLOT(setCompletionText(QString)) );
        }
    }

    m_state = state;
}


void Nepomuk::StatementEditor::Private::_k_propertyActivated( const Nepomuk::Types::Property& prop )
{
    m_selectedProperty = prop;
    setState( EditingValue );
}


void Nepomuk::StatementEditor::Private::_k_returnPressed()
{
    // we handle some special types
    Nepomuk::Variant v( Soprano::LiteralValue::fromString( m_lineEdit->text(), m_selectedProperty.literalRangeType().dataTypeUri() ).variant() );

    if ( m_resource.isValid() ) {
        m_resource.addProperty( m_selectedProperty, v );
    }

    emit q->statementCreated( m_selectedProperty, v );
    setState( EditingProperty );
}


void Nepomuk::StatementEditor::Private::_k_dateTimeEditingFinished()
{
    if ( m_resource.isValid() )
        m_resource.addProperty( m_selectedProperty, m_dateTimeEdit->dateTime() );
    emit q->statementCreated( m_selectedProperty, m_dateTimeEdit->dateTime() );
    setState( EditingProperty );
}


void Nepomuk::StatementEditor::Private::_k_resourceActivated( const Nepomuk::Resource& res )
{
    if ( m_resource.isValid() )
        m_resource.addProperty( m_selectedProperty, res );
    emit q->statementCreated( m_selectedProperty, res );
    setState( EditingProperty );
}


void Nepomuk::StatementEditor::Private::_k_typeActivated( const Nepomuk::Types::Class& type )
{
    if ( m_resource.isValid() )
        m_resource.addType( type.uri() );
    emit q->statementCreated( m_selectedProperty, Resource( type.uri() ) ); // this is a bit ugly
    setState( EditingProperty );
}


Nepomuk::StatementEditor::StatementEditor( QWidget* parent )
    : QWidget( parent ),
      d( new Private() )
{
    d->q = this;

    d->m_propertyLabel = new QLabel( this );
    d->m_lineEdit = new KLineEdit( this );
    d->m_dateTimeEdit = new QDateTimeEdit( this );
    QHBoxLayout* lay = new QHBoxLayout( this );
    lay->setMargin( 0 );
    lay->addWidget( d->m_propertyLabel );
    lay->addWidget( d->m_lineEdit );
    lay->addWidget( d->m_dateTimeEdit );

    d->m_propertyCompleter = new PropertyCompleter( this );
    d->m_resourceCompleter = new ResourceCompletion( this );
    d->m_typeCompleter = new TypeCompletion( this );
    d->m_propertyCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    d->m_resourceCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    d->m_typeCompleter->setCaseSensitivity(Qt::CaseInsensitive);

    d->m_lineEdit->installEventFilter( this );

    d->setState( EditingProperty );
}


Nepomuk::StatementEditor::~StatementEditor()
{
    delete d;
}


void Nepomuk::StatementEditor::setResource( const Nepomuk::Resource& res )
{
    d->m_resource = res;
}


bool Nepomuk::StatementEditor::eventFilter( QObject* watched, QEvent* event )
{
    if ( watched == d->m_lineEdit &&
         event->type() == QEvent::KeyPress ) {
        kDebug() << event;
        QKeyEvent* ke = static_cast<QKeyEvent*>( event );
        if ( ke->key() == Qt::Key_Escape ) {
            // this will go back one step
            if ( d->m_state == EditingValue ) {
                d->setState( EditingProperty );
                return true;
            }
        }
    }

    return QWidget::eventFilter( watched, event );
}

#include "statementeditor.moc"
