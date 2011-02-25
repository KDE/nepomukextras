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


#include "resourceselectiondialog.h"
#include "simpleresourcemodel.h"

#include <Nepomuk/Resource>
#include <Nepomuk/Query/Query>
#include <QLabel>

#include <KLocale>

#include <QListView>
#include <QVBoxLayout>


class Nepomuk::ResourceSelectionDialog::Private
{
public:
    void _k_selectionChanged();

    QLabel* m_textLabel;
    Utils::SimpleResourceModel* m_resourceModel;
    QListView* m_resourceView;

    ResourceSelectionDialog* q;
};


void Nepomuk::ResourceSelectionDialog::Private::_k_selectionChanged()
{
    q->enableButton( KDialog::Ok, !m_resourceView->selectionModel()->selectedRows().isEmpty() );
}


Nepomuk::ResourceSelectionDialog::ResourceSelectionDialog(QWidget* parent)
        : KDialog(parent),
        d(new Private())
{
    d->q = this;

    d->m_textLabel = new QLabel( i18nc("@info label above a list view","Please select a resource"), mainWidget() );
    d->m_textLabel->setWordWrap( true );
    d->m_resourceView = new QListView( mainWidget() );
    d->m_resourceModel = new Utils::SimpleResourceModel( this );
    d->m_resourceView->setModel(d->m_resourceModel);

    QVBoxLayout* layout = new QVBoxLayout(mainWidget());
    layout->addWidget(d->m_textLabel);
    layout->addWidget(d->m_resourceView);

    connect( d->m_resourceView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
             this, SLOT(_k_selectionChanged()) );
    d->_k_selectionChanged();
}


Nepomuk::ResourceSelectionDialog::~ResourceSelectionDialog()
{
    delete d;
}


void Nepomuk::ResourceSelectionDialog::setText( const QString& text )
{
    d->m_textLabel->setText( text );
}


void Nepomuk::ResourceSelectionDialog::setResources( const QList<Resource>& resources )
{
    d->m_resourceModel->setResources(resources);
}


Nepomuk::Resource Nepomuk::ResourceSelectionDialog::selectedResource() const
{
    return d->m_resourceModel->resourceForIndex(d->m_resourceView->selectionModel()->currentIndex());
}


// static
Nepomuk::Resource Nepomuk::ResourceSelectionDialog::selectResource( QWidget* parent, const QString& text, const QList< Nepomuk::Resource >& candidates, const QString& caption )
{
    ResourceSelectionDialog dlg( parent );
    dlg.setText(text);
    if (!caption.isEmpty())
        dlg.setCaption(caption);
    dlg.setResources(candidates);
    if (dlg.exec() == KDialog::Ok)
        return dlg.selectedResource();
    else
        return Resource();
}


// static
Nepomuk::Resource Nepomuk::ResourceSelectionDialog::selectResource( QWidget* parent, const QString& text, const Nepomuk::Query::Query& query, const QString& caption )
{
    // TODO!
    return Nepomuk::Resource();
}

#include "resourceselectiondialog.moc"


