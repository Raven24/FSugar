#include <QtGui>

#include "contactmodel.h"

bool compareContactsGreaterThan(const Contact *c1, const Contact *c2)
{
	return c1->lastName > c2->lastName;
}

ContactModel::ContactModel(QObject *parent) :
		QAbstractTableModel(parent)
{
}

int ContactModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent)
	return contacts->size();
}

int ContactModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent)
	return 4;
}

QVariant ContactModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Vertical) {
		return QVariant();
	}

	switch (role) {
		case 0:
			switch (section) {
				case 0:
					return QString(tr("Name"));
				case 1:
					return QString(tr("Telefon"));
				case 2:
					return QString(tr("E-Mail"));
				case 3:
					return QString(tr("Fax"));
			}
		case 13:
			switch (section) {
				case 0:
				case 1:
				case 2:
				case 3:
					return QSize(200, 20);
			}
	}
	return QVariant();
}

QVariant ContactModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid()) {
		return QVariant();
	}

	switch (role) {
		case Qt::DisplayRole:
		case Qt::ToolTipRole:
			switch(index.column()) {
				case 0:
					return QString(contacts->at(index.row())->lastName).append(", ").append(contacts->at(index.row())->firstName);
				case 1:
					return contacts->at(index.row())->phoneWork;
				case 2:
					return contacts->at(index.row())->email1;
				case 3:
					return contacts->at(index.row())->phoneFax;
			}
		case Qt::FontRole:
			QFont f;
			f.setPixelSize(10);
			return f;
	}

	return QVariant();
}

void ContactModel::read(QList<Contact *> *_contacts)
{
	contacts = _contacts;
	processNewContact();
}

Contact* ContactModel::newContact()
{
	Contact *_contact = new Contact();
	contacts->append(_contact);
	connect(_contact, SIGNAL(saved()),
			this, SLOT(processNewContact()));
	return _contact;
}

void ContactModel::processNewContact()
{
	sortData();
	reset();
}

void ContactModel::sortData()
{
	qSort(contacts->begin(), contacts->end(), compareContactsGreaterThan);
}
