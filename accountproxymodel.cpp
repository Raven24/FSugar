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

#include <QtCore>
#include "accountproxymodel.h"

AccountProxyModel::AccountProxyModel(QObject *parent) :
		QSortFilterProxyModel(parent)
{
}

/*const QHash<int, QByteArray> & AccountProxyModel::roleNames() const
{
	qDebug() << "roleNames() called";
	return sourceModel()->roleNames();
}

void AccountProxyModel::setRoleNames(const QHash<int, QByteArray> &roleNames)
{
	sourceModel()->setRoleNames(roleNames);
}

QVariant AccountProxyModel::data(const QModelIndex &index, int role)
{
	qDebug() << index;
	qDebug() << role;
	return QVariant();
}*/
