#include <QtCore>
#include <QtGui>

#include "accountdetail.h"
#include "accountmodel.h"
#include "account.h"
#include "notesmodel.h"

AccountDetail::AccountDetail(QWidget *parent) :
	QWidget(parent)
{
	qDebug() << "something's wrong (missing parameter in constructor)";
}

AccountDetail::AccountDetail(const QModelIndex *index)
{
	retrieveAccount(index);
	fillData();

	connect(save, SIGNAL(pressed()),
			this, SLOT(saveChanges()));
}

void AccountDetail::retrieveAccount(const QModelIndex *index)
{
	//qDebug() << index->row();

	AccountModel *model = AccountModel::getInstance();
	acc = model->getAccount(index->row());
	connect(acc, SIGNAL(notesAvailable()),
			this, SLOT(displayNotes()));
	acc->getNotes();
}

void AccountDetail::paintEvent(QPaintEvent *)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

}

void AccountDetail::fillData()
{
	//qDebug() << "supposed to fill in data now";

	QVBoxLayout *layout = new QVBoxLayout();

	QFormLayout *address = new QFormLayout();
	address->setLabelAlignment(Qt::AlignRight);
	accountName = new QLineEdit(acc->name);
	QFont f = accountName->font();
	f.setBold(true); f.setPixelSize(14);
	accountName->setFont(f);

	accountAddress1	= new QLineEdit(acc->address_street);
	accountAddress2	= new QLineEdit(acc->address_postalcode);
	accountAddress3	= new QLineEdit(acc->address_city);
	accountAddress4	= new QLineEdit(acc->address_country);
	accountEmail		= new QLineEdit(acc->email);
	accountWebsite	= new QLineEdit(acc->website);
	address->addRow(tr("Adresse"), accountAddress1);
	address->addRow(tr("PLZ"), accountAddress2);
	address->addRow(tr("Ort"), accountAddress3);
	address->addRow(tr("Land"), accountAddress4);
	//address->addSpacing(1);
	address->addRow(tr("Email"), accountEmail);
	address->addRow(tr("Homepage"), accountWebsite);

	QFormLayout *phone = new QFormLayout();
	phone->setLabelAlignment(Qt::AlignRight);
	accountPhone = new QLineEdit(acc->phone_office);
	accountFax = new QLineEdit(acc->phone_fax);
	accountPhoneAlt = new QLineEdit(acc->phone_alternate);
	phone->addRow(tr("Tel"), accountPhone);
	phone->addRow(tr("Fax"), accountFax);
	phone->addRow(tr("Alt"), accountPhoneAlt);

	QHBoxLayout *itemsContainer = new QHBoxLayout();
	save = new QPushButton(QIcon(":save.png"), tr("Speichern"));
	newNote = new QPushButton(QIcon(":notes.png"), tr("Neue Notiz"));
	newDocument = new QPushButton(QIcon(":documents.png"), tr("Neues Dokument"));
	itemsContainer->addWidget(save, 0, Qt::AlignLeft);
	itemsContainer->addWidget(newNote, 3, Qt::AlignRight);
	itemsContainer->addWidget(newDocument, 0, Qt::AlignRight);

	QHBoxLayout *contactInfo = new QHBoxLayout();
	contactInfo->addLayout(address);
	contactInfo->addLayout(phone);

	QLabel *accountDescLbl = new QLabel(tr("Beschreibung"));
	accountDescription = new QTextEdit(acc->description);

	notesTable = new QTableView();
	notesTable->verticalHeader()->hide();
	notesTable->horizontalHeader()->setStretchLastSection(true);
	notesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	notesTable->setSelectionMode(QAbstractItemView::SingleSelection);

	layout->addWidget(accountName, 2);
	layout->addLayout(contactInfo, 3);
	layout->addWidget(accountDescLbl);
	layout->addWidget(accountDescription, 1);
	layout->addStretch(1);
	layout->addLayout(itemsContainer, 1);
	layout->addWidget(notesTable, 2);

	setLayout(layout);
	qApp->setStyleSheet("AccountDetail { background: qradialgradient(cx:0, cy:-0.2, radius: 1, fx:0.5, fy:0.5, stop:0 #888, stop:1 #555); } "
						"QLabel { color: rgba(255, 255, 255, 60%); font-size: 9px; } "
						"QLineEdit, QTextEdit { background: #FdFdFd; border: 1px solid qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop:0 white, stop: 1 #989889); } "
						"QPushButton { border: none; background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #343434, stop: 0.7 #222, stop: 1 #121212); color: #FFF; border-radius: 4px; padding: 5px; }"
						"QPushButton:pressed { background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #121212, stop: 0.7 #222, stop: 1 #343434); }"
					   );
}

void AccountDetail::displayNotes()
{
	//qDebug() << "notes:" << acc->notes.size();
	notesTable->setModel(new NotesModel(acc));
	notesTable->resizeRowsToContents();
}

void AccountDetail::saveChanges()
{
	acc->name = accountName->text();

	acc->address_street = accountAddress1->text();
	acc->address_postalcode = accountAddress2->text();
	acc->address_city = accountAddress3->text();
	acc->address_country = accountAddress4->text();

	acc->email = accountEmail->text();
	acc->website = accountWebsite->text();

	acc->phone_office = accountPhone->text();
	acc->phone_fax = accountFax->text();
	acc->phone_alternate = accountPhoneAlt->text();

	acc->description = accountDescription->toPlainText();

	qDebug() << acc->toString();
}
