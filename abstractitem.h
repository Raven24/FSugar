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

#ifndef ABSTRACTITEM_H
#define ABSTRACTITEM_H

#include <QtCore>
#include "note.h"

class AbstractItem : public QObject
{

	Q_OBJECT

public:
    explicit AbstractItem(QObject *parent = 0);

	QString id, description, category, type;

	QDateTime date_entered, date_modified;

	virtual QList<Note*>* getNotesList();
	QList<Note*> notes;
	SugarCrm *crm;

signals:
	void saved();
	void notesAvailable();

public slots:
	virtual void save() = 0;
	void populateNotes(QString _id);
	void getChildren();
	void getNotes();
	void gotCreated(QString _id);
	void seeWhoSaved(QString _id);

	QString toString();

};

#endif // ABSTRACTITEM_H
