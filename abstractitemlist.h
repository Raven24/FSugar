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

#ifndef ABSTRACTITEMLIST_H
#define ABSTRACTITEMLIST_H

#include <QtGui>
#include "searchfield.h"

class AbstractItemList : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractItemList(QWidget *parent = 0);
	//virtual void setModel(QSortFilterProxyModel *model) = 0;
	virtual QSortFilterProxyModel* getModel() = 0;

signals:

public slots:
	virtual void openDetail(const QModelIndex index) = 0;
	void closeDetail(const int index);
	void filterList(QString phrase);

protected:
	QListView *listView;
	QTabWidget *tabView;
	QLabel *welcomeText;
	SearchField *search;
	QStackedLayout *stack;

};

#endif // ABSTRACTITEMLIST_H
