#ifndef ACCOUNTDETAIL_H
#define ACCOUNTDETAIL_H

#include <QtCore>
#include <QtGui>

#include "account.h"
#include "createnotedialog.h"

class AccountDetail : public QWidget
{
	Q_OBJECT

public:
	AccountDetail(QWidget *parent = 0);
	AccountDetail(const QModelIndex *index);
	AccountDetail(Account *_acc);

public slots:
	void displayNotes();
	void saveChanges();
	void createNewNote();
	void progress(bool p = false);
	void afterSaveAct();

private:
	void paintEvent(QPaintEvent *);
	void retrieveAccount(const QModelIndex *index);
	void fillData();
	void initDialog();

	Account *acc;
	QTableView *notesTable;
	bool inProgress;

	QLineEdit *accountName, *accountAddress1,
		*accountAddress2, *accountAddress3,
		*accountAddress4, *accountEmail,
		*accountWebsite, *accountPhone,
		*accountFax, *accountPhoneAlt;

	QTextEdit *accountDescription;

	QPushButton *save, *newNote,
		*newDocument;

	QLabel *loading;

	CreateNoteDialog *newNoteDialog;
	NotesModel *notesModel;
	SugarCrm *crm;
};

#endif // ACCOUNTDETAIL_H
