#include <QtCore>

#include "notesmodel.h"
#include "account.h"

NotesModel::NotesModel(QObject *parent) :
		QAbstractTableModel(parent)
{
}

NotesModel::NotesModel(Account *acc, QObject *parent) :
		QAbstractTableModel(parent), acc(acc)
{
}

int NotesModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent)
	return acc->notes.size();
}

int NotesModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent)
	return 3;
}

QVariant NotesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Vertical) {
		return QVariant();
	}
	//qDebug() << "section header" << section << "role" << role;

	switch(role) {
		case 0:
			switch(section) {
				case 0:
					return QString(tr("Name"));
				case 1:
					return QString(tr("Datum"));
				case 2:
					return QString(tr("Beschreibung"));
			}
		case 13:
			switch(section) {
				case 0:
					return QSize(150, 20);
				case 1:
					return QSize(170, 20);
				case 2:
					return QSize(200, 20);
			}
	}

	return QVariant();
}

QVariant NotesModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid() ) {
		return QVariant();
	}

	switch (role) {
		case Qt::DisplayRole:
		case Qt::ToolTipRole:
			switch(index.column()) {
				case 0:
					return acc->notes.at(index.row())->name;
				case 1:
					return acc->notes.at(index.row())->date_modified.toString("d. MMM yyyy, HH:mm");
				case 2:
					return acc->notes.at(index.row())->description;
			}
	}

	return QVariant();
}
