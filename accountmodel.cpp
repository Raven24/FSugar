#include <QtGui>
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

	connect(crm, SIGNAL(dataAvailable()),
			this, SLOT(processData()));
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
	if(!accounts.isEmpty()) {
		Account *acc = accounts[index.row()];

		switch(role) {
			case 16:
			case Qt::DisplayRole:
				//qDebug() << "name requested";
				return acc->name.simplified();
			case 32:
				//qDebug() << "address requested";
				return acc->address_street.simplified();
			case 64:
				//qDebug() << "address2 requested";
				return acc->address_postalcode.append(", ").append(acc->address_city).simplified();
			case 128:
				//qDebug() << "phone requested";
				return acc->phone_office.simplified();
		}

		return QVariant();
	}
	qDebug() << QString("test: %1, %2").arg(index.row()).arg(role);
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
				accounts[index.row()]->setName(newVal);
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

void AccountModel::processData()
{
	qDebug() << "processing data";

	accounts.clear();
	QMapIterator<QString, QMap<QString, QString> > i(crm->entries);

	while (i.hasNext()) {
		i.next();
		Account *tmp = new Account();

		//qDebug() << i.value().value("id");
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

		accounts.append(tmp);
	}

	qSort(accounts.begin(), accounts.end(), compareAccountsLessThan);
	//emit dataChanged(QModelIndex(), QModelIndex());
	reset();
	//qDebug() << accounts.size();
	emit dataReady();
}
