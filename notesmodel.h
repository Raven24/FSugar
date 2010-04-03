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

#ifndef NOTESMODEL_H
#define NOTESMODEL_H

#include <QAbstractTableModel>
#include <QtCore>

#include "account.h"
#include "note.h"


class NotesModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	NotesModel(QObject *parent = 0);
	//NotesModel(Account *acc, QObject *parent = 0);

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	QVariant headerData ( int section, Qt::Orientation  orientation, int role = Qt::DisplayRole ) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

	void read(QList<Note*> *_notes);

	Note* newNote();

public slots:
	void processNewNote();

private:
	QList<Note* > *notes;

	void sortData();
};

#endif // NOTESMODEL_H
