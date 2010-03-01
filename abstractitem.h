#ifndef ABSTRACTITEM_H
#define ABSTRACTITEM_H

#include <QtCore>
#include "note.h"

class AbstractItem : public QObject
{

	Q_OBJECT

public:
    explicit AbstractItem(QObject *parent = 0);

	QString id, description, category;

	QDateTime date_entered, date_modified;

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
