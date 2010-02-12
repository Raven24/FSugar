#ifndef ACCOUNTMODEL_H
#define ACCOUNTMODEL_H

#include <QAbstractListModel>
#include "sugarcrm.h"
#include "account.h"

class AccountModel : public QAbstractListModel
{
Q_OBJECT
public:
    explicit AccountModel(QObject *parent = 0);

	static AccountModel *getInstance();

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) ;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role);
	Qt::ItemFlags flags(const QModelIndex &index);

	void debug();
	Account* getAccount(const int num);

signals:
	void dataReady();

public slots:
	void fetchData();
	void processData();

private:
	SugarCrm *crm;
	QList<Account*> accounts;

	static AccountModel *instance;
};

#endif // ACCOUNTMODEL_H
