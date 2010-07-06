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

#ifndef ACCOUNTDETAIL_H
#define ACCOUNTDETAIL_H

#include <QtCore>
#include <QtGui>

#include "account.h"
#include "notesmodel.h"
#include "contactmodel.h"
#include "createnotedialog.h"
#include "abstractitemdetail.h"

class AccountDetail : public AbstractItemDetail
{
	Q_OBJECT

public:
	AccountDetail(QWidget *parent = 0);
	AccountDetail(const QModelIndex *index);
	AccountDetail(Account *_acc);

	Account* getItem();

public slots:
	void displayContacts();
	void saveChanges();
	void initDialog();
	void addContact();
	void openContact(QModelIndex _index);

private:
	void retrieveItem(const QModelIndex *_index);
	void fillData();
	void hideButtons(bool _var);

	QTableView *contactsTable;
	QTabWidget *childrenTab;

	QLineEdit *accountName, *accountAddress1,
		*accountAddress2, *accountAddress3,
		*accountAddress4, *accountEmail,
		*accountWebsite, *accountPhone,
		*accountFax, *accountPhoneAlt;

	QTextEdit *accountDescription;

	QCheckBox *catChkBox;
	QPushButton *newContact, *openEmailBtn;

	Account *item;

	ContactModel *contactsModel;
};

#endif // ACCOUNTDETAIL_H
