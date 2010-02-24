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
	//qDebug() << "inside Note::save()";

	if (!fileName.isEmpty() && id.isEmpty())
		prepareAttachment();

	crm->createNote("Notes",
					name,
					description,
					parentType,
					parentId);
}

void Note::afterSave(const QString _id)
{
	//qDebug() << "inside Note::afterSave()";
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
		//qDebug() << "fetch attachment here!";
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
