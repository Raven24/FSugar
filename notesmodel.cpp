#include <QtCore>
#include <QtGui>
#include "qfont.h"

#include "notesmodel.h"
#include "account.h"

bool compareNotesGreaterThan(const Note *n1, const Note *n2)
{
	return n1->date_modified > n2->date_modified;
}

NotesModel::NotesModel(QObject *parent) :
		QAbstractTableModel(parent)
{
}

int NotesModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent)
	return notes->size();
}

int NotesModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent)
	return 4;
}

QVariant NotesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Vertical) {
		return QVariant();
	}

	switch(role) {
		case 0:
			switch(section) {
				case 0:
					return QString(tr("Name"));
				case 1:
					return QString(tr("Datum"));
				case 2:
					return QString(tr("Attachment"));
				case 3:
					return QString(tr("Beschreibung"));
			}
		case 13:
			switch(section) {
				case 0:
					return QSize(150, 20);
				case 1:
				case 2:
					return QSize(170, 20);
				case 3:
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
					return notes->at(index.row())->name;
				case 1:
					return notes->at(index.row())->date_modified.toString("d. MMM yyyy, HH:mm");
				case 2:
					return notes->at(index.row())->fileName;
				case 3:
					return notes->at(index.row())->description;
			}
		case Qt::FontRole:
			QFont f;
			f.setPixelSize(10);
			switch(index.column()) {
				case 0:
					f.setBold(true);
					return f;
				case 1:
				case 2:
					f.setPixelSize(9);
					return f;
				case 3:
					return f;
			}
	}

	return QVariant();
}

void NotesModel::read(QList<Note *> *_notes)
{
	notes = _notes;
	processNewNote();
}

Note* NotesModel::newNote()
{
	Note *_note = new Note();
	notes->append(_note);
	connect(_note, SIGNAL(saved()),
			this, SLOT(processNewNote()));
	return _note;
}

void NotesModel::processNewNote()
{
	sortData();
	reset();
}

void NotesModel::sortData()
{
	qSort(notes->begin(), notes->end(), compareNotesGreaterThan);
}
