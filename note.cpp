#include <QObject>

#include "note.h"
#include "sugarcrm.h"

Note::Note(QObject *parent) :
		QObject(parent)
{
	crm = SugarCrm::getInstance();

	connect(crm, SIGNAL(entryCreated(QString)),
			this, SLOT(afterSave(QString)));
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
		emit noteSaved();
		return;
	}

	if(!id.isEmpty()) return;

	id = _id;

	if(gotAttachment) {
		crm->setNoteAttachment(id, fileName, fileData);
		gotAttachment = false;
	} else {
		emit noteSaved();
	}

}

void Note::prepareAttachment()
{
	QFileInfo fi(fileName);
	QFile file(fileName);
	if(!file.open(QIODevice::ReadOnly))
		qDebug() << "unable to open file";

	QByteArray ba = file.readAll();
	fileData = ba.toBase64();
	fileName = fi.fileName();
	gotAttachment = true;
}
