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
