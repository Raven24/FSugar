#ifndef CONTACTDETAIL_H
#define CONTACTDETAIL_H

#include <QtCore>
#include <QtGui>

#include "contact.h"
#include "notesmodel.h"
#include "createnotedialog.h"

class ContactDetail : public QWidget
{
Q_OBJECT

public:
	ContactDetail(QWidget *parent = 0);
	ContactDetail(const QModelIndex *index);
	ContactDetail(Contact *_contact);

public slots:
	void displayNotes();
	void saveChanges();
	void createNewNote();
	void progress(bool p = false);
	void endProgress();
	void afterSaveAct();
	void showNewDocumentDialog();
	void showNewNoteDialog();
	void downloadNoteAttachment(const QModelIndex _index);

private:
	void paintEvent(QPaintEvent *);
	void retrieveContact();
	void fillData();
	void initDialog();
	void hideButtons(const bool _var);

	Contact *contact;
	QTableView *notesTable;
	bool inProgress;

	QLineEdit *firstNameEdit, *lastNameEdit,
		*phoneHomeEdit, *phoneMobileEdit, *phoneWorkEdit,
		*phoneOtherEdit, *phoneFaxEdit,
		*email1Edit, *email2Edit,
		*addressStreetEdit, *addressCityEdit, *addressPostalcodeEdit,
		*addressCountryEdit, *leadSourceEdit;

	QTextEdit *contactDescription;

	QPushButton *save, *newNote, *newDocument;
	QLabel *loading;

	CreateNoteDialog *newNoteDialog;
	NotesModel *notesModel;
	SugarCrm *crm;

};

#endif // CONTACTDETAIL_H
