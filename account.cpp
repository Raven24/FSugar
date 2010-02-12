#include <QtGui>

#include "sugarcrm.h"
#include "account.h"

Account::Account(QObject *parent) :
		QObject(parent)
{
	crm = SugarCrm::getInstance();
}

void Account::setName(const QString _name)
{
	name = _name.simplified();
}

void Account::getNotes()
{
	connect(crm, SIGNAL(notesAvailable()),
			this, SLOT(populateNotes()));

	crm->getRelatedNotes("Accounts", id);
}

void Account::populateNotes()
{
	notes.clear();
	QMapIterator<QString, QMap<QString, QString> > i(crm->notes);

	while(i.hasNext()) {
		i.next();

		Note *tmp = new Note();
		tmp->id = i.value().value("id");
		tmp->name = i.value().value("name");
		tmp->description = i.value().value("description");
		tmp->date_entered = QDateTime::fromString(i.value().value("date_entered"), "yyyy-MM-dd hh:mm:ss");
		tmp->date_modified = QDateTime::fromString(i.value().value("date_modified"), "yyyy-MM-dd hh:mm:ss");

		notes.append(tmp);
	}
	emit notesAvailable();
}

bool Account::operator<(const Account *other) const
{
	qDebug() << "comparing accounts" << name << "and" << other->name;
	if(name < other->name)
		return true;

	return false;
}

QString Account::toString()
{
	return QString("Account %1: \n\tName: %2\n\t"
				   "Address: %3, %4, %5, %6\n\t"
				   "Phone: %7, %8, %9\n\t"
				   "%10, %11\n\t"
				   "Desc: %12")
			.arg(id, name, address_street, address_postalcode)
			.arg(address_city, address_country, phone_office, phone_fax)
			.arg(phone_alternate, email, website, description);
}
