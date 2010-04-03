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

#include <QObject>
#include <QtGui>

#include "note.h"
#include "sugarcrm.h"
#include "mainwindow.h"

Note::Note(QObject *parent) :
		QObject(parent)
{
	crm = SugarCrm::getInstance();

	connect(crm, SIGNAL(entryCreated(QString)),
			this, SLOT(afterSave(QString)));
	connect(crm, SIGNAL(attachmentAvailable(QString)),
			this, SLOT(checkAttachment(QString)));
}

void Note::save()
{
	if (!fileName.isEmpty() && id.isEmpty())
		prepareAttachment();

	crm->createNote("Notes",
					name,
					description,
					parentType,
					parentId,
					contactId);
}

void Note::afterSave(const QString _id)
{
	if(_id == id) {
		emit saved();
		return;
	}

	if(!id.isEmpty()) return;

	id = _id;

	if(gotAttachment) {
		crm->setNoteAttachment(id, fileName, fileData);
		gotAttachment = false;
	} else {
		emit saved();
	}

}

/**
 * convert the file to a base64 string
 */
void Note::prepareAttachment()
{
	QFileInfo fi(fileName);
	QFile file(fileName);
	if(!file.open(QIODevice::ReadOnly)) {
		qDebug() << "unable to open file";
		return;
	}

	QByteArray ba = file.readAll();
	fileData = ba.toBase64();
	fileName = fi.fileName();
	gotAttachment = true;
}

/**
 * download and open the attachment
 */
void Note::downloadAttachment()
{
	QByteArray ba;

	if(fileData.isEmpty()) {
		crm->getNoteAttachment(id);
		return;
	} else {
		ba = QByteArray::fromBase64(fileData.toAscii());
	}

	QDir::setCurrent(QDesktopServices::storageLocation(QDesktopServices::TempLocation));
	QFile f(fileName);
	if (!f.open(QIODevice::ReadWrite)) {
		qDebug() << "unable to save file";
		// TODO: in case this happens, make the directory chooseable
		return;
	}
	f.write(ba);
	QFileInfo fi(f);
	QString localFilePath = fi.absoluteFilePath();

	// reset the directory
	QDir::setCurrent(MainWindow::appPath);

	// open the just created file in the associated application
	emit openingAttachment();
	if(!QDesktopServices::openUrl(QUrl::fromLocalFile(localFilePath))) {
		QMessageBox msg;
		msg.setText(tr("Die Datei wurde unter \n%1 gespeichert.").arg(QDesktopServices::TempLocation));
		msg.setIcon(QMessageBox::Information);
		msg.exec();
	}
}

void Note::checkAttachment(const QString _id)
{
	if (id != _id) return;

	fileData = crm->noteAttachment;
	downloadAttachment();
}
