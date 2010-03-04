#ifndef ABSTRACTITEMDETAIL_H
#define ABSTRACTITEMDETAIL_H

#include <QtGui>

#include "abstractitem.h"
#include "sugarcrm.h"
#include "notesmodel.h"
#include "createnotedialog.h"

class AbstractItemDetail : public QWidget
{
Q_OBJECT
public:
	AbstractItemDetail(QWidget *parent = 0);
	AbstractItemDetail(const QModelIndex *index);
	AbstractItemDetail(AbstractItem *_item);

	virtual AbstractItem* getItem() = 0;

public slots:
	virtual void displayNotes();
	virtual void createNewNote();

	virtual void saveChanges() = 0;
	virtual void afterSaveAct();

	void progress(bool _p = false);
	void endProgress();

	void showNewDocumentDialog();
	void showNewNoteDialog();
	void downloadNoteAttachment(const QModelIndex _index);

	virtual void initDialog();

protected:
	virtual void retrieveItem();
	virtual void fillData();
	virtual void hideButtons(const bool _var);
	void paintEvent(QPaintEvent *);

	bool inProgress;
	QTableView *notesTable;
	QLabel *loading;
	QPushButton *save, *newNote,
		*newDocument;

	SugarCrm *crm;
	CreateNoteDialog *newNoteDialog;
	NotesModel *notesModel;

};

#endif // ABSTRACTITEMDETAIL_H
