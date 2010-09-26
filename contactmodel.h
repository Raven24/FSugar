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

#ifndef CONTACTMODEL_H
#define CONTACTMODEL_H

#include <QAbstractTableModel>
#include "contact.h"

class ContactModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	ContactModel(QObject *parent = 0);

	static ContactModel *getInstance();

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

	void read(QList<Contact*> *_contacts);
	Contact* newContact();
	Contact* getContact(const int num);

signals:

public slots:
	void processNewContact();

private:
	QList<Contact*> *contacts;

	static ContactModel *instance;

	void sortData();

};

#endif // CONTACTMODEL_H
