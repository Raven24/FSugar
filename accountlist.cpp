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

#include "accountlist.h"
#include "accountdetail.h"

AccountList::AccountList(QWidget *parent) :
    QWidget(parent)
{
	// initialize members
	accountList = new QListView();
	accountTabs = new QTabWidget();
	welcomeText = new QLabel(tr("Waehlen sie in der Liste einen Eintrag aus, um hier die dazugehoerigen Details anzuzeigen."));
	search		= new SearchField();

	accountList->setAlternatingRowColors(true);
	accountTabs->setTabsClosable(true);

	// layout
	QHBoxLayout *layout = new QHBoxLayout();
	QVBoxLayout *left = new QVBoxLayout();
	stack = new QStackedLayout();

	left->setSpacing(4);
	left->addWidget(search);
	left->addWidget(accountList);

	stack->addWidget(accountTabs);
	stack->addWidget(welcomeText);
	stack->setCurrentWidget(welcomeText);

	layout->setSpacing(0);
	layout->addLayout(left, 1);
	layout->addLayout(stack, 2);

	setLayout(layout);

	// connect signals and slots
	connect(accountList, SIGNAL(doubleClicked(QModelIndex)),
			this, SLOT(openAccountDetails(QModelIndex)));
	connect(search, SIGNAL(searchPhraseChanged(QString)),
			this, SLOT(filterAccountList(QString)));
	connect(accountTabs, SIGNAL(tabCloseRequested(int)),
			this, SLOT(closeAccountTab(int)));
}

void AccountList::setModel(AccountProxyModel *model)
{
	accountModel = model;
	accountList->setModel(accountModel);
}

void AccountList::openAccountDetails(const QModelIndex index)
{
	QModelIndex num(accountModel->mapToSource(index));
	qDebug() << "[app] now's a good time to open the info for " << num.row();
	AccountDetail *item = new AccountDetail(&num);
	accountTabs->setCurrentIndex(accountTabs->addTab(item, item->getItem()->name));

	// make the tabwidget visible
	stack->setCurrentWidget(accountTabs);
}

void AccountList::filterAccountList(QString phrase)
{
	accountModel->setFilterRegExp(QRegExp(phrase, Qt::CaseInsensitive, QRegExp::FixedString));
}

void AccountList::closeAccountTab(const int index)
{
	QWidget *tab = accountTabs->widget(index);
	accountTabs->removeTab(index);
	if(!tab->property("doNotDelete").toBool()) {
		tab->deleteLater();
	}
	if(accountTabs->count() < 1) {
		stack->setCurrentWidget(welcomeText);
	}
}
