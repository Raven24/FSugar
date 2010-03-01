#ifndef CONTACTMODEL_H
#define CONTACTMODEL_H

#include <QAbstractTableModel>
#include "contact.h"

class ContactModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	ContactModel(QObject *parent = 0);

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

	void read(QList<Contact*> *_contacts);
	Contact* newContact();

signals:

public slots:
	void processNewContact();

private:
	QList<Contact*> *contacts;

	void sortData();

};

#endif // CONTACTMODEL_H
