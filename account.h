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

#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QtCore>

#include "abstractitem.h"
#include "sugarcrm.h"
#include "note.h"
#include "contact.h"

class Account : public AbstractItem
{
	Q_OBJECT

public:
	Account(QObject *parent = 0);

	QString name,
		address_city, address_street, address_country, address_postalcode, address_state,
		phone_fax, phone_alternate, phone_office,
		email, website;

	QList<Contact*> contacts;

	bool operator<(const Account *other) const;

signals:
	void contactsAvailable();

public slots:
	void save();
	void openEmail();
	void getChildren();
	void populateContacts(const QString _id);

private:
	void getContacts();

};

#endif // ACCOUNT_H

