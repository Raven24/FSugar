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

#include <QtGui>

#include "sugarcrm.h"
#include "abstractitem.h"
#include "account.h"
#include "note.h"
#include "notesmodel.h"

Account::Account(QObject *parent) :
		AbstractItem(parent)
{
	type = "Accounts";
}

bool Account::operator<(const Account *other) const
{
	qDebug() << "comparing accounts" << name << "and" << other->name;
	if(name < other->name)
		return true;

	return false;
}

void Account::save()
{
	crm->updateAccount(id, name, description, address_street, address_city,
					   address_postalcode, address_country, phone_office,
					   phone_fax, phone_alternate, email, website, category);
}

void Account::getChildren()
{
	AbstractItem::getChildren();
	getContacts();
}

void Account::getContacts()
{
	contacts.clear();
	connect(crm, SIGNAL(dataAvailable(QString)),
			this, SLOT(populateContacts(QString)));
	crm->getEntryList("Contacts",
					  QString("account_id = \"%1\"").arg(id),
					  "last_name",
					  0,
					  70,
					  0);
}

void Account::openEmail()
{
	useAttribute(email, "email");
}

void Account::gotoWebsite()
{
	useAttribute(website, "website");
}

void Account::populateContacts(const QString _id)
{
	if(_id.isEmpty()) emit contactsAvailable();
	if(_id != id) return;

	contacts.clear();

	QMap<QString, QMap<QString, QString> >::const_iterator i = crm->entries->begin();

	while (i != crm->entries->end()) {
		Contact *tmp = new Contact();
		tmp->id = i.value().value("id");
		tmp->firstName = i.value().value("first_name");
		tmp->lastName = i.value().value("last_name");
		tmp->description = i.value().value("description");
		tmp->phoneWork = i.value().value("phone_work");
		tmp->phoneFax = i.value().value("phone_fax");
		tmp->phoneHome = i.value().value("phone_home");
		tmp->phoneMobile = i.value().value("phone_mobile");
		tmp->email1 = i.value().value("email1");
		tmp->email2 = i.value().value("email2");
		tmp->addressStreet = i.value().value("primary_address_street");
		tmp->addressCity = i.value().value("primary_address_city");
		tmp->addressPostalcode = i.value().value("primary_address_postalcode");
		tmp->addressCountry = i.value().value("primary_address_country");

		contacts.append(tmp);
		i++;
	}

	emit contactsAvailable();
}
