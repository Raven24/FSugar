#include <QtCore>
#include "accountproxymodel.h"

AccountProxyModel::AccountProxyModel(QObject *parent) :
		QSortFilterProxyModel(parent)
{
}

const QHash<int, QByteArray> & AccountProxyModel::roleNames() const
{
	qDebug() << "roleNames() called";
	return sourceModel()->roleNames();
}

/*void AccountProxyModel::setRoleNames(const QHash<int, QByteArray> &roleNames)
{
	sourceModel()->setRoleNames(roleNames);
}*/

QVariant AccountProxyModel::data(const QModelIndex &index, int role)
{
	qDebug() << index;
	qDebug() << role;
	return QVariant();
}
