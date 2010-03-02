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

public slots:
	void displayContacts();
	void saveChanges();
	void initDialog();

private:
	void retrieveItem(const QModelIndex *_index);
	void fillData();
	QTableView *contactsTable;

	QLineEdit *accountName, *accountAddress1,
		*accountAddress2, *accountAddress3,
		*accountAddress4, *accountEmail,
		*accountWebsite, *accountPhone,
		*accountFax, *accountPhoneAlt;

	QTextEdit *accountDescription;

	QCheckBox *catChkBox;

	Account *item;

	ContactModel *contactsModel;
};

#endif // ACCOUNTDETAIL_H
