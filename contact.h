#ifndef CONTACT_H
#define CONTACT_H

#include <QtCore>
#include "sugarcrm.h"
#include "note.h"
#include "abstractitem.h"

class Contact : public AbstractItem
{

	Q_OBJECT

public:
    explicit Contact(QObject *parent = 0);

	QString saltutation, firstName, lastName,
		phoneHome, phoneMobile, phoneWork, phoneOther, phoneFax,
		email1, email2,
		addressStreet, addressCity, addressState, addressPostalcode, addressCountry,
		leadSource, accountName, accountId;

	QDateTime birthdate;

signals:

public slots:
	void save();

};

#endif // CONTACT_H
