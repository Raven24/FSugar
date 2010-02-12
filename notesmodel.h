#ifndef NOTESMODEL_H
#define NOTESMODEL_H

#include <QAbstractTableModel>

#include "account.h"

class NotesModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	NotesModel(QObject *parent = 0);
	NotesModel(Account *acc, QObject *parent = 0);

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	QVariant headerData ( int section, Qt::Orientation  orientation, int role = Qt::DisplayRole ) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;


private:
	Account *acc;
};

#endif // NOTESMODEL_H
