#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QtCore>

#include "sugarcrm.h"
#include "note.h"

class Account : public QObject
{
	Q_OBJECT

public:
	Account(QObject *parent = 0);

	QString id, name, description;

	QString address_city, address_street,
		address_country, address_postalcode,
		address_state;

	QString phone_fax, phone_alternate,
		phone_office;

	QString email, website;

	QList<Note*> notes;

	void setName(const QString _name);

	void getNotes();
	QString toString();

	bool operator<(const Account *other) const;

signals:
	void notesAvailable();

public slots:
	void populateNotes();

private:
	SugarCrm *crm;
};

#endif // ACCOUNT_H

