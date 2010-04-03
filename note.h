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

#ifndef NOTE_H
#define NOTE_H

#include <QtCore>
#include "sugarcrm.h"

class Note : public QObject
{
Q_OBJECT
public:
	Note(QObject *parent = 0);

	QString id, name, description,
	parentType, parentId, contactId,
	fileName, fileData;

	QDateTime date_entered, date_modified;

	void save();

signals:
	void saved();
	void openingAttachment();

public slots:
	void afterSave(const QString _id);
	void downloadAttachment();
	void checkAttachment(const QString _id);

private:
	SugarCrm *crm;
	bool gotAttachment;

	void prepareAttachment();
};

#endif // NOTE_H
