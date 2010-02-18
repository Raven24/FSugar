#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QtCore>

#include "sugarcrm.h"
#include "note.h"
#include "notesmodel.h"

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

	QString toString();

	bool operator<(const Account *other) const;

signals:
	void notesAvailable();
	void saved();

public slots:
	void populateNotes(QString _id);
	void getNotes();
	void seeWhoSaved(QString _id);
	void save();
	void gotCreated(QString _id);

private:
	SugarCrm *crm;

};

#endif // ACCOUNT_H

