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
	parentType, parentId,
	fileName, fileData;

	QDateTime date_entered, date_modified;

	void save();

signals:
	void saved();

public slots:
	void afterSave(const QString _id);

private:
	SugarCrm *crm;
	bool gotAttachment;

	void prepareAttachment();
};

#endif // NOTE_H
