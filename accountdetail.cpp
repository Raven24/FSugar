#include <QtCore>
#include <QtGui>

#include "accountdetail.h"
#include "accountmodel.h"
#include "account.h"
#include "notesmodel.h"
#include "createnotedialog.h"
#include "abstractitemdetail.h"

AccountDetail::AccountDetail(QWidget *parent) :
	AbstractItemDetail(parent)
{
	qDebug() << "AccountDetail(): something's wrong (missing parameter in constructor)";
}

AccountDetail::AccountDetail(const QModelIndex *index)
{
	initDialog();
	retrieveItem(index);
	fillData();
}

AccountDetail::AccountDetail(Account *_acc)
{
	item = _acc;

	connect(item, SIGNAL(notesAvailable()),
			this, SLOT(displayNotes()));
	connect(item, SIGNAL(contactsAvailable()),
			this, SLOT(displayContacts()));
	connect(item, SIGNAL(saved()),
			this, SLOT(afterSaveAct()));

	initDialog();
	hideButtons(true);
	progress(false);
}

void AccountDetail::initDialog()
{
	AbstractItemDetail::initDialog();

	//initialize models
	contactsModel = new ContactModel(this);

	// layouts
	QVBoxLayout *layout = new QVBoxLayout();
	QVBoxLayout *rightLayout = new QVBoxLayout();
	QFormLayout *address = new QFormLayout();
	QFormLayout *phone = new QFormLayout();
	QFormLayout *catLayout = new QFormLayout();
	QHBoxLayout *itemsContainer = new QHBoxLayout();
	QHBoxLayout *contactInfo = new QHBoxLayout();

	// edit fields
	accountName = new QLineEdit();
	QFont f = accountName->font();
	f.setBold(true); f.setPixelSize(14);
	accountName->setFont(f);

	accountAddress1 = new QLineEdit();
	accountAddress2 = new QLineEdit();
	accountAddress3 = new QLineEdit();
	accountAddress4 = new QLineEdit();
	accountEmail = new QLineEdit();
	accountWebsite = new QLineEdit();
	accountPhone = new QLineEdit();
	accountFax = new QLineEdit();
	accountPhoneAlt = new QLineEdit();
	accountDescription = new QTextEdit();
	catChkBox = new QCheckBox();

	QLabel *accountDescLbl = new QLabel(tr("Beschreibung"));
	QTabWidget *childrenTab = new QTabWidget(this);

	// layout population
	address->setLabelAlignment(Qt::AlignRight);
	address->addRow(tr("Adresse"), accountAddress1);
	address->addRow(tr("PLZ"), accountAddress2);
	address->addRow(tr("Ort"), accountAddress3);
	address->addRow(tr("Land"), accountAddress4);
	address->addRow(tr("Email"), accountEmail);
	address->addRow(tr("Homepage"), accountWebsite);

	catLayout->addRow(tr("Pressekontakt"), catChkBox);

	phone->setLabelAlignment(Qt::AlignRight);
	phone->addRow(tr("Tel"), accountPhone);
	phone->addRow(tr("Fax"), accountFax);
	phone->addRow(tr("Alt"), accountPhoneAlt);

	rightLayout->addLayout(phone);
	rightLayout->addLayout(catLayout);

	itemsContainer->addWidget(save, 0, Qt::AlignLeft);
	itemsContainer->addStretch(3);
	itemsContainer->addWidget(loading, 0, Qt::AlignCenter);
	itemsContainer->addStretch(3);
	itemsContainer->addWidget(newNote, 0, Qt::AlignRight);
	itemsContainer->addWidget(newDocument, 0, Qt::AlignRight);

	contactInfo->addLayout(address);
	contactInfo->addLayout(rightLayout);

	contactsTable = new QTableView(this);
	contactsTable->verticalHeader()->hide();
	contactsTable->horizontalHeader()->setStretchLastSection(true);
	contactsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	contactsTable->setSelectionMode(QAbstractItemView::SingleSelection);

	childrenTab->addTab(notesTable, QIcon(":notes.png"), tr("Notizen"));
	childrenTab->addTab(contactsTable, QIcon(":login.png"), tr("Kontakte"));

	layout->addWidget(accountName, 1);
	layout->addLayout(contactInfo, 1);
	layout->addWidget(accountDescLbl, 0, Qt::AlignTop);
	layout->addWidget(accountDescription, 1, Qt::AlignTop);
	layout->addStretch(3);
	layout->addLayout(itemsContainer, 1);
	layout->addWidget(childrenTab, 3);

	setLayout(layout);
}

void AccountDetail::retrieveItem(const QModelIndex *index)
{
	//qDebug() << index->row();

	AccountModel *model = AccountModel::getInstance();
	item = model->getAccount(index->row());

	connect(crm, SIGNAL(entryCreated(QString)),
			item, SLOT(getNotes()));
	connect(item, SIGNAL(saved()),
			this, SLOT(afterSaveAct()));
	connect(item, SIGNAL(notesAvailable()),
			this, SLOT(displayNotes()));
	connect(item, SIGNAL(contactsAvailable()),
			this, SLOT(displayContacts()));

	item->getChildren();
}

void AccountDetail::fillData()
{
	//qDebug() << "supposed to fill in data now";
	accountName->setText(item->name);
	accountAddress1->setText(item->address_street);
	accountAddress2->setText(item->address_postalcode);
	accountAddress3->setText(item->address_city);
	accountAddress4->setText(item->address_country);
	accountEmail->setText(item->email);
	accountWebsite->setText(item->website);

	accountPhone->setText(item->phone_office);
	accountFax->setText(item->phone_fax);
	accountPhoneAlt->setText(item->phone_alternate);

	accountDescription->setText(item->description);

	if(item->category.contains(QRegExp("press", Qt::CaseInsensitive, QRegExp::FixedString))) {
		catChkBox->setChecked(true);
	}
}

void AccountDetail::displayContacts()
{
	contactsModel->read(&item->contacts);
	contactsTable->setModel(contactsModel);
	contactsTable->resizeRowsToContents();
	progress(false);
}

void AccountDetail::saveChanges()
{
	progress(true);
	item->name = accountName->text();

	item->address_street = accountAddress1->text();
	item->address_postalcode = accountAddress2->text();
	item->address_city = accountAddress3->text();
	item->address_country = accountAddress4->text();

	item->email = accountEmail->text();
	item->website = accountWebsite->text();

	item->phone_office = accountPhone->text();
	item->phone_fax = accountFax->text();
	item->phone_alternate = accountPhoneAlt->text();

	item->description = accountDescription->toPlainText();

	if(catChkBox->isChecked()) {
		item->category = "presse";
	} else {
		item->category = "";
	}

	//qDebug() << acc->toString();

	item->save();
}
