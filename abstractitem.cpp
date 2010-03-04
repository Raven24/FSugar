#include <QtCore>
#include "abstractitem.h"

AbstractItem::AbstractItem(QObject *parent) :
    QObject(parent)
{
	crm = SugarCrm::getInstance();
	connect(crm, SIGNAL(entryUpdated(QString)),
			this, SLOT(seeWhoSaved(QString)));
	connect(crm, SIGNAL(entryCreated(QString)),
			this, SLOT(gotCreated(QString)));
}

void AbstractItem::getChildren()
{
	getNotes();
}

void AbstractItem::getNotes()
{
	notes.clear();
	connect(crm, SIGNAL(notesAvailable(QString)),
			this, SLOT(populateNotes(QString)));

	//qDebug() << toString() << "wants notes";

	crm->getRelatedNotes(type, id);
}

void AbstractItem::populateNotes(QString _id)
{
	qDebug() << _id << "we got notes for you...";

	if(_id.isEmpty()) emit notesAvailable();
	if(_id != id) return;

	notes.clear();
	QMapIterator<QString, QMap<QString, QString> > i(crm->notes);

	while(i.hasNext()) {
		i.next();

		Note *tmp = new Note();
		tmp->id = i.value().value("id");
		tmp->name = i.value().value("name");
		tmp->description = i.value().value("description");
		tmp->fileName = i.value().value("filename");
		tmp->parentId = i.value().value("parent_id");
		tmp->parentType = i.value().value("parent_type");
		tmp->date_entered = QDateTime::fromString(i.value().value("date_entered"), "yyyy-MM-dd hh:mm:ss");
		tmp->date_modified = QDateTime::fromString(i.value().value("date_modified"), "yyyy-MM-dd hh:mm:ss");

		notes.append(tmp);
	}

	emit notesAvailable();
}

QString AbstractItem::toString()
{
	return QString("Item %1, type %2").arg(id, type);
}

QList<Note*>* AbstractItem::getNotesList()
{
	return &notes;
}

void AbstractItem::seeWhoSaved(QString _id)
{
	//qDebug() << _id;
	if(id == _id){
		emit saved();
	}
}

void AbstractItem::gotCreated(QString _id)
{
	if(id.isEmpty()) {
		id = _id;
		emit saved();
	}
}
