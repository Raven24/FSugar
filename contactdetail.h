#ifndef CONTACTDETAIL_H
#define CONTACTDETAIL_H

#include <QtCore>
#include <QtGui>

#include "contact.h"
#include "notesmodel.h"
#include "createnotedialog.h"
#include "abstractitemdetail.h"

class ContactDetail : public AbstractItemDetail
{
Q_OBJECT

public:
	ContactDetail(QWidget *parent = 0);
	ContactDetail(const QModelIndex *index);
	ContactDetail(Contact *_contact);

public slots:
	void saveChanges();
	void initDialog();

private:
	void retrieveContact();
	void fillData();

	Contact *item;

	QLineEdit *firstNameEdit, *lastNameEdit,
		*phoneHomeEdit, *phoneMobileEdit, *phoneWorkEdit,
		*phoneOtherEdit, *phoneFaxEdit,
		*email1Edit, *email2Edit,
		*addressStreetEdit, *addressCityEdit, *addressPostalcodeEdit,
		*addressCountryEdit, *leadSourceEdit;

	QTextEdit *contactDescription;

};

#endif // CONTACTDETAIL_H
