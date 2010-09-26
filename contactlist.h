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

#ifndef CONTACTLIST_H
#define CONTACTLIST_H

#include "abstractitemlist.h"
#include "contactproxymodel.h"

class ContactList : public AbstractItemList
{
	Q_OBJECT

public:
	explicit ContactList(QWidget *parent = 0);
	void setModel(ContactProxyModel *model);
	ContactProxyModel* getModel();

public slots:
	void openDetail(const QModelIndex index);

protected:
	ContactProxyModel *itemsModel;
};

#endif // CONTACTLIST_H
