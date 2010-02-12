#ifndef ACCOUNTPROXYMODEL_H
#define ACCOUNTPROXYMODEL_H

#include <QSortFilterProxyModel>

class AccountProxyModel : public QSortFilterProxyModel
{
public:
	AccountProxyModel(QObject *parent = 0);
	const QHash<int, QByteArray> & roleNames() const;
	//void setRoleNames(const QHash<int, QByteArray> &roleNames);
	QVariant data(const QModelIndex &index, int role);
};

#endif // ACCOUNTPROXYMODEL_H
