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
	void getChildren();
	void populateContacts(const QString _id);

private:
	void getContacts();

};

#endif // ACCOUNT_H

