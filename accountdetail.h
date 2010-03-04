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
	QPushButton *newContact;

	Account *item;

	ContactModel *contactsModel;
};

#endif // ACCOUNTDETAIL_H
