/************************************************
 *
 * Copyright © 2009-2010 Florian Staudacher
 * <florian_staudacher@yahoo.de>
 *
 *
 * This file is part of FSugar.
 * 
 * FSugar is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FSugar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with FSugar. If not, see <http://www.gnu.org/licenses/>.
 *
 ***********************************************/

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

	Account* newAccount();
	void debug();
	Account* getAccount(const int num);

signals:
	void dataReady();

public slots:
	void fetchData();
	void processData(const QString _id);
	void processNewAccount();

private:
	SugarCrm *crm;
	QList<Account*> accounts;

	static AccountModel *instance;

	void sortData();
};

#endif // ACCOUNTMODEL_H
