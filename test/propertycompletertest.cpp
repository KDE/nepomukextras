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

#include <KApplication>
#include <KCmdLineArgs>
#include <KAboutData>
#include <KUrl>
#include <KLineEdit>

#include <QtGui/QVBoxLayout>

#include "propertycompleter.h"
#include "typecompletion.h"


int main( int argc, char *argv[] )
{
    KAboutData aboutData( "propertycompletertest", "propertycompletertest",
                          ki18n("Nepomuk Property Completer Test"),
                          "0.1",
                          ki18n("Nepomuk Property Completer Test"),
                          KAboutData::License_GPL,
                          ki18n("(c) 2009, Sebastian Trüg"),
                          KLocalizedString(),
                          "http://nepomuk.kde.org" );
    aboutData.addAuthor(ki18n("Sebastian Trüg"),ki18n("Maintainer"), "trueg@kde.org");
    aboutData.setProgramIconName( "nepomuk" );

    KCmdLineArgs::init( argc, argv, &aboutData );

    KCmdLineOptions options;
    KCmdLineArgs::addCmdLineOptions( options );

    KApplication app;

    QWidget w;
    KLineEdit* domainEdit = new KLineEdit( &w );
    KLineEdit* propertyEdit = new KLineEdit( &w );
    QVBoxLayout* layout = new QVBoxLayout( &w );
    layout->addWidget( domainEdit );
    layout->addWidget( propertyEdit );

    Nepomuk::TypeCompletion* domainComp = new Nepomuk::TypeCompletion();
    domainComp->setCaseSensitivity(Qt::CaseInsensitive);
    domainEdit->setCompleter( domainComp );

    Nepomuk::PropertyCompleter* propComp = new Nepomuk::PropertyCompleter();
    propComp->setCaseSensitivity(Qt::CaseInsensitive);
    propertyEdit->setCompleter( propComp );

    QObject::connect( domainEdit, SIGNAL( textEdited( QString ) ),
                      domainComp, SLOT( setCompletionText( QString ) ) );
    QObject::connect( propertyEdit, SIGNAL( textEdited( QString ) ),
                      propComp, SLOT( setCompletionText( QString ) ) );
    QObject::connect( domainComp, SIGNAL( typeActivated( Nepomuk::Types::Class ) ),
                      propComp, SLOT( setDomain( Nepomuk::Types::Class ) ) );
    w.show();

    return app.exec();
}
