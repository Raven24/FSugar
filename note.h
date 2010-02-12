#ifndef NOTE_H
#define NOTE_H

#include <QtCore>

class Note : public QObject
{
Q_OBJECT
public:
	Note(QObject *parent = 0);

	QString id, name, description;

	QDateTime date_entered, date_modified;
};

#endif // NOTE_H
