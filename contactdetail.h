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

	Contact* getItem();

public slots:
	void saveChanges();
	void initDialog();
	void retrieveContact(Contact* _c);
	void createNewNote();

private:
	void fillData();

	Contact *item;

	QLineEdit *firstNameEdit, *lastNameEdit,
		*phoneHomeEdit, *phoneMobileEdit, *phoneWorkEdit,
		*phoneOtherEdit, *phoneFaxEdit,
		*email1Edit, *email2Edit,
		*addressStreetEdit, *addressCityEdit, *addressPostalcodeEdit,
		*addressCountryEdit, *leadSourceEdit;

	QTextEdit *contactDescriptionEdit;

	QPushButton *openEmailBtn, *openEmail2Btn;

};

#endif // CONTACTDETAIL_H
