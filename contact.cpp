#include "contact.h"

Contact::Contact(QObject *parent) :
	AbstractItem(parent)
{
	type = "Contacts";
}

void Contact::save()
{
	crm->updateContact(id, firstName, lastName, description,
					   addressStreet, addressPostalcode, addressCity, addressCountry,
					   phoneWork, phoneHome, phoneMobile, phoneFax, phoneOther,
					   email1, email2,
					   accountId, accountName);
	setProperty("newEntry", false);
}
