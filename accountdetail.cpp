#include <QtCore>
#include <QtGui>

#include "accountdetail.h"
#include "accountmodel.h"
#include "account.h"
#include "notesmodel.h"
#include "createnotedialog.h"

AccountDetail::AccountDetail(QWidget *parent) :
	QWidget(parent)
{
	qDebug() << "AccountDetail(): something's wrong (missing parameter in constructor)";
}

AccountDetail::AccountDetail(const QModelIndex *index)
{
	initDialog();
	retrieveAccount(index);
	fillData();
}

AccountDetail::AccountDetail(Account *_acc)
{
	acc = _acc;

	connect(acc, SIGNAL(notesAvailable()),
			this, SLOT(displayNotes()));
	connect(acc, SIGNAL(saved()),
			this, SLOT(afterSaveAct()));
	connect(acc, SIGNAL(notesAvailable()),
			this, SLOT(displayNotes()));

	initDialog();
	newNote->hide();
	progress(false);
}

void AccountDetail::initDialog()
{
	newNoteDialog = new CreateNoteDialog(this);
	newNoteDialog->hide();
	notesModel = new NotesModel(this);
	loading = new QLabel(this);
	QMovie *mov = new QMovie();
	mov->setCacheMode(QMovie::CacheAll);
	loading->setMovie(mov);
	mov->setFileName(":loading.gif");
	mov->setScaledSize(QSize(20, 20));
	mov->start();

	crm = SugarCrm::getInstance();

	// layouts
	QVBoxLayout *layout = new QVBoxLayout();
	QFormLayout *address = new QFormLayout();
	QFormLayout *phone = new QFormLayout();
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

	QLabel *accountDescLbl = new QLabel(tr("Beschreibung"));

	// buttons
	save = new QPushButton(QIcon(":save.png"), tr("Speichern"));
	newNote = new QPushButton(QIcon(":notes.png"), tr("Neue Notiz"));
	newDocument = new QPushButton(QIcon(":documents.png"), tr("Neues Dokument"));

	// layout population
	address->setLabelAlignment(Qt::AlignRight);
	address->addRow(tr("Adresse"), accountAddress1);
	address->addRow(tr("PLZ"), accountAddress2);
	address->addRow(tr("Ort"), accountAddress3);
	address->addRow(tr("Land"), accountAddress4);
	address->addRow(tr("Email"), accountEmail);
	address->addRow(tr("Homepage"), accountWebsite);

	phone->setLabelAlignment(Qt::AlignRight);
	phone->addRow(tr("Tel"), accountPhone);
	phone->addRow(tr("Fax"), accountFax);
	phone->addRow(tr("Alt"), accountPhoneAlt);

	itemsContainer->addWidget(save, 0, Qt::AlignLeft);
	itemsContainer->addStretch(3);
	itemsContainer->addWidget(loading, 0, Qt::AlignCenter);
	itemsContainer->addStretch(3);
	itemsContainer->addWidget(newNote, 0, Qt::AlignRight);
	//itemsContainer->addWidget(newDocument, 0, Qt::AlignRight);

	contactInfo->addLayout(address);
	contactInfo->addLayout(phone);

	notesTable = new QTableView(this);
	notesTable->verticalHeader()->hide();
	notesTable->horizontalHeader()->setStretchLastSection(true);
	notesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	notesTable->setSelectionMode(QAbstractItemView::SingleSelection);

	layout->addWidget(accountName, 2);
	layout->addLayout(contactInfo, 3);
	layout->addWidget(accountDescLbl);
	layout->addWidget(accountDescription, 1);
	layout->addStretch(2);
	layout->addLayout(itemsContainer, 1);
	layout->addWidget(notesTable, 2);

	setLayout(layout);

	connect(save, SIGNAL(pressed()),
			this, SLOT(saveChanges()));
	connect(newNote, SIGNAL(pressed()),
			newNoteDialog, SLOT(show()));
	connect(newNoteDialog, SIGNAL(accepted()),
			this, SLOT(createNewNote()));
}

void AccountDetail::retrieveAccount(const QModelIndex *index)
{
	//qDebug() << index->row();

	AccountModel *model = AccountModel::getInstance();
	acc = model->getAccount(index->row());
	connect(acc, SIGNAL(notesAvailable()),
			this, SLOT(displayNotes()));
	connect(crm, SIGNAL(entryCreated(QString)),
			acc, SLOT(getNotes()));
	connect(acc, SIGNAL(saved()),
			this, SLOT(afterSaveAct()));
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

void AccountDetail::afterSaveAct()
{
	progress(false);
	newNote->show();

	acc->getNotes();
}

void AccountDetail::fillData()
{
	//qDebug() << "supposed to fill in data now";
	accountName->setText(acc->name);
	accountAddress1->setText(acc->address_street);
	accountAddress2->setText(acc->address_postalcode);
	accountAddress3->setText(acc->address_city);
	accountAddress4->setText(acc->address_country);
	accountEmail->setText(acc->email);
	accountWebsite->setText(acc->website);

	accountPhone->setText(acc->phone_office);
	accountFax->setText(acc->phone_fax);
	accountPhoneAlt->setText(acc->phone_alternate);

	accountDescription->setText(acc->description);
}

void AccountDetail::displayNotes()
{
	//qDebug() << "notes:" << acc->notes.size();
	notesModel->read(&acc->notes);
	notesTable->setModel(notesModel);
	notesTable->resizeRowsToContents();
	progress(false);
}

void AccountDetail::saveChanges()
{
	progress(true);
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

	//qDebug() << acc->toString();

	acc->save();
}

void AccountDetail::createNewNote()
{
	progress(true);
	//qDebug() << "should create a note right about here";

	connect(crm, SIGNAL(entryCreated(QString)),
			acc, SLOT(getNotes()));

	crm->createNote("Notes",
					newNoteDialog->noteName,
					newNoteDialog->noteDescription,
					"Accounts",
					acc->id);
}

void AccountDetail::progress(bool p)
{
	if(p) loading->show();
	else loading->hide();
}
