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

#include "contactlist.h"
#include "contactdetail.h"

ContactList::ContactList(QWidget *parent) :
		AbstractItemList(parent)
{
}

void ContactList::setModel(ContactProxyModel *model)
{
	itemsModel = model;
	listView->setModel(itemsModel);
}

ContactProxyModel* ContactList::getModel()
{
	return itemsModel;
}

void ContactList::openDetail(const QModelIndex index)
{
	QModelIndex num(getModel()->mapToSource(index));
	qDebug() << "[app] now's a good time to open the info for " << num.row();
	ContactDetail *item = new ContactDetail(&num);
	tabView->setCurrentIndex(tabView->addTab(item, QString(item->getItem()->firstName).append(" ").append(item->getItem()->lastName)));

	// make the tabwidget visible
	stack->setCurrentWidget(tabView);
}
