#include <QtGui>

#include "sugarcrm.h"
#include "account.h"
#include "note.h"
#include "notesmodel.h"

bool compareNotesGreaterThan(const Note *n1, const Note *n2)
{
	return n1->date_modified > n2->date_modified;
}

Account::Account(QObject *parent) :
		QObject(parent)
{
	crm = SugarCrm::getInstance();
	connect(crm, SIGNAL(entryUpdated(QString)),
			this, SLOT(seeWhoSaved(QString)));
	connect(crm, SIGNAL(entryCreated(QString)),
			this, SLOT(gotCreated(QString)));
}

void Account::setName(const QString _name)
{
	name = _name.simplified();
}

void Account::getNotes()
{
	notes.clear();
	connect(crm, SIGNAL(notesAvailable(QString)),
			this, SLOT(populateNotes(QString)));

	crm->getRelatedNotes("Accounts", id);
}

void Account::populateNotes(QString _id)
{
	if(_id.isEmpty()) emit notesAvailable();
	if(_id != id) return;

	notes.clear();
	QMapIterator<QString, QMap<QString, QString> > i(crm->notes);

	while(i.hasNext()) {
		i.next();

		Note *tmp = new Note();
		tmp->id = i.value().value("id");
		tmp->name = i.value().value("name");
		tmp->description = i.value().value("description");
		tmp->fileName = i.value().value("filename");
		tmp->parentId = i.value().value("parent_id");
		tmp->parentType = i.value().value("parent_type");
		tmp->date_entered = QDateTime::fromString(i.value().value("date_entered"), "yyyy-MM-dd hh:mm:ss");
		tmp->date_modified = QDateTime::fromString(i.value().value("date_modified"), "yyyy-MM-dd hh:mm:ss");

		notes.append(tmp);
	}
	qSort(notes.begin(), notes.end(), compareNotesGreaterThan);
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

void Account::save()
{
	crm->updateAccount(id, name, description, address_street, address_city,
					   address_postalcode, address_country, phone_office,
					   phone_fax, phone_alternate, email, website);
}

void Account::seeWhoSaved(QString _id)
{
	//qDebug() << _id;
	if(id == _id){
		emit saved();
	}
}

void Account::gotCreated(QString _id)
{
	if(id.isEmpty()) {
		id = _id;
		emit saved();
	}
}
