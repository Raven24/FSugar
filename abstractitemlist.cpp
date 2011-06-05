/************************************************
 *
 * Copyright © 2009-2010 Florian Staudacher
 * <florian_staudacher@yahoo.de>
 *
 *
 * This file is part of FSugar.
 * 
 * FSugar is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FSugar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with FSugar. If not, see <http://www.gnu.org/licenses/>.
 *
 ***********************************************/

#include "abstractitemlist.h"
#include "abstractitemdetail.h"

AbstractItemList::AbstractItemList(QWidget *parent) :
    QWidget(parent)
{
	// initialize members
	listView = new QListView();
	tabView  = new QTabWidget();
	welcomeText = new QLabel(tr("Waehlen sie in der Liste einen Eintrag aus, \num hier die dazugehoerigen Details anzuzeigen."));
	welcomeText->setAlignment(Qt::AlignCenter);
	search		= new SearchField();

	listView->setAlternatingRowColors(true);
	tabView->setTabsClosable(true);

	// layout
	QHBoxLayout *layout = new QHBoxLayout();
	QVBoxLayout *left = new QVBoxLayout();
	stack = new QStackedLayout();

	left->setSpacing(4);
	left->addWidget(search);
	left->addWidget(listView);

	stack->addWidget(tabView);
	stack->addWidget(welcomeText);
	stack->setCurrentWidget(welcomeText);

	layout->setSpacing(0);
	layout->addLayout(left, 1);
	layout->addLayout(stack, 2);

	setLayout(layout);

	// connect signals and slots
	connect(listView, SIGNAL(doubleClicked(QModelIndex)),
			this, SLOT(openDetail(QModelIndex)));
	connect(search, SIGNAL(searchPhraseChanged(QString)),
			this, SLOT(filterList(QString)));
	connect(tabView, SIGNAL(tabCloseRequested(int)),
			this, SLOT(closeDetail(int)));
}

void AbstractItemList::filterList(QString phrase)
{
	getModel()->setFilterRegExp(QRegExp(phrase, Qt::CaseInsensitive, QRegExp::FixedString));
}

void AbstractItemList::closeDetail(const int index)
{
	QWidget *tab = tabView->widget(index);
	tabView->removeTab(index);
	if(!tab->property("doNotDelete").toBool()) {
		tab->deleteLater();
	}
	if(tabView->count() < 1) {
		stack->setCurrentWidget(welcomeText);
	}
}
