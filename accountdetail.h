#ifndef ACCOUNTDETAIL_H
#define ACCOUNTDETAIL_H

#include <QtCore>
#include <QtGui>

#include "account.h"

class AccountDetail : public QWidget
{
	Q_OBJECT

public:
	AccountDetail(QWidget *parent = 0);
	AccountDetail(const QModelIndex *index);

public slots:
	void displayNotes();
	void saveChanges();

private:
	void paintEvent(QPaintEvent *);
	void retrieveAccount(const QModelIndex *index);
	void fillData();

	Account *acc;
	QTableView *notesTable;

	QLineEdit *accountName, *accountAddress1,
		*accountAddress2, *accountAddress3,
		*accountAddress4, *accountEmail,
		*accountWebsite, *accountPhone,
		*accountFax, *accountPhoneAlt;

	QTextEdit *accountDescription;

	QPushButton *save, *newNote,
		*newDocument;
};

#endif // ACCOUNTDETAIL_H
