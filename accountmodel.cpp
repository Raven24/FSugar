#include <QtGui>
#include "account.h"
#include "accountmodel.h"
#include "sugarcrm.h"

bool compareAccountsLessThan(const Account *a1, const Account *a2)
{
	return (a1->name < a2->name);
}

AccountModel *AccountModel::instance = NULL;

AccountModel::AccountModel(QObject *parent) :
	QAbstractListModel(parent)
{
	QHash<int, QByteArray> roles;
	roles.insert(16, "name");
	roles.insert(32, "address");
	roles.insert(64, "address2");
	roles.insert(128, "telephone");

	setRoleNames(roles);

	crm = SugarCrm::getInstance();

	connect(crm, SIGNAL(dataAvailable(QString)),
			this, SLOT(processData(QString)));
}

AccountModel *AccountModel::getInstance()
{
	if(AccountModel::instance == NULL) {
		AccountModel::instance = new AccountModel();
	}
	return AccountModel::instance;
}

int AccountModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent)

	if(!accounts.isEmpty()) {
		return accounts.size();
	}
	return 0;
}

int AccountModel::columnCount(const QModelIndex &parent)
{
	Q_UNUSED(parent)
	return 1;
}

QVariant AccountModel::data(const QModelIndex &index, int role) const
{
	if(accounts.isEmpty()) {
		return QVariant();
	}

	Account *acc = accounts[index.row()];

	switch(role) {
		case 16:
		case Qt::DisplayRole:
			return acc->name.simplified();
		case 32:
			return acc->address_street.simplified();
		case 64:
			return acc->address_postalcode.append(", ").append(acc->address_city).simplified();
		case 128:
			return acc->phone_office.simplified();
		case 256:
			return acc->category;
	}

	return QVariant();
}

Qt::ItemFlags AccountModel::flags(const QModelIndex &index)
{
	qDebug() << "flags requested:" << index.row();
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

bool AccountModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	qDebug() << "trying to set data";

	if(index.row() < accounts.size() && index.row() >= 0) {
		QString newVal(value.toString());

		switch(role) {
			case 16:
				accounts[index.row()]->name = newVal;
				return true;
		/*	case 32:
				accounts.at(index.row()).address_street = newVal;
				return true;
			case 64:
				accounts.at(index.row()).address_postalcode = newVal;
				return true;
			case 128:
				accounts.at(index.row()).name = newVal;
				return true;*/
		}


		emit dataChanged(index, index);
		return true;
	}

	return false;

}

Account* AccountModel::getAccount(const int num)
{
	return accounts[num];
}

void AccountModel::fetchData()
{

	crm->getEntryList("Accounts");
}

void AccountModel::processData(const QString _id)
{
	if(!_id.isEmpty()) return;
	if (crm->entries->size() < 1) return;

	accounts.clear();
	QMap<QString, QMap<QString, QString> >::const_iterator i = crm->entries->begin();

	while (i != crm->entries->end()) {
		Account *tmp = new Account();

		tmp->id = i.value().value("id");
		tmp->name = i.value().value("name");
		tmp->address_street = i.value().value("billing_address_street");
		tmp->address_city = i.value().value("billing_address_city");
		tmp->address_postalcode = i.value().value("billing_address_postalcode");
		tmp->address_country = i.value().value("billing_address_country");
		tmp->phone_office = i.value().value("phone_office");
		tmp->phone_fax = i.value().value("phone_fax");
		tmp->phone_alternate = i.value().value("phone_alternate");
		tmp->email = i.value().value("email1");
		tmp->website = i.value().value("website");
		tmp->description = i.value().value("description");
		tmp->category = i.value().value("kategorie_c");

		accounts.append(tmp);
		i++;
	}
	sortData();
	reset();

	emit dataReady();
}

Account* AccountModel::newAccount()
{
	Account *_acc = new Account();
	accounts.append(_acc);
	connect(_acc, SIGNAL(saved()),
			this, SLOT(processNewAccount()));
	return _acc;
}

void AccountModel::processNewAccount()
{
	sortData();
	reset();
}

void AccountModel::sortData()
{
	qSort(accounts.begin(), accounts.end(), compareAccountsLessThan);
}
