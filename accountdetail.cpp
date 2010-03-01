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
	connect(acc, SIGNAL(contactsAvailable()),
			this, SLOT(displayContacts()));
	connect(acc, SIGNAL(saved()),
			this, SLOT(afterSaveAct()));

	initDialog();
	hideButtons(true);
	progress(false);
}

void AccountDetail::initDialog()
{
	newNoteDialog = new CreateNoteDialog(this);
	newNoteDialog->hide();

	//initialize models
	notesModel = new NotesModel(this);
	contactsModel = new ContactModel(this);

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

	notesTable = new QTableView(this);
	notesTable->verticalHeader()->hide();
	notesTable->horizontalHeader()->setStretchLastSection(true);
	notesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	notesTable->setSelectionMode(QAbstractItemView::SingleSelection);

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

	connect(save, SIGNAL(pressed()),
			this, SLOT(saveChanges()));
	connect(newNote, SIGNAL(pressed()),
			this, SLOT(showNewNoteDialog()));
	connect(newNoteDialog, SIGNAL(accepted()),
			this, SLOT(createNewNote()));
	connect(newDocument, SIGNAL(pressed()),
			this, SLOT(showNewDocumentDialog()));
	connect(notesTable, SIGNAL(doubleClicked(QModelIndex)),
			this, SLOT(downloadNoteAttachment(QModelIndex)));
}

void AccountDetail::retrieveAccount(const QModelIndex *index)
{
	//qDebug() << index->row();

	AccountModel *model = AccountModel::getInstance();
	acc = model->getAccount(index->row());

	connect(crm, SIGNAL(entryCreated(QString)),
			acc, SLOT(getNotes()));
	connect(acc, SIGNAL(saved()),
			this, SLOT(afterSaveAct()));
	connect(acc, SIGNAL(notesAvailable()),
			this, SLOT(displayNotes()));
	connect(acc, SIGNAL(contactsAvailable()),
			this, SLOT(displayContacts()));

	acc->getChildren();
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
	hideButtons(false);

	acc->getChildren();
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

	if(acc->category.contains(QRegExp("press", Qt::CaseInsensitive, QRegExp::FixedString))) {
		catChkBox->setChecked(true);
	}
}

void AccountDetail::displayNotes()
{
	//qDebug() << "notes:" << acc->notes.size();
	notesModel->read(&acc->notes);
	notesTable->setModel(notesModel);
	notesTable->resizeRowsToContents();
	progress(false);
}

void AccountDetail::displayContacts()
{
	contactsModel->read(&acc->contacts);
	contactsTable->setModel(contactsModel);
	contactsTable->resizeRowsToContents();
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

	if(catChkBox->isChecked()) {
		acc->category = "presse";
	} else {
		acc->category = "";
	}

	//qDebug() << acc->toString();

	acc->save();
}

void AccountDetail::createNewNote()
{
	progress(true);
	//qDebug() << "should create a note right about here";

	Note *newNote = notesModel->newNote();
	newNote->name = newNoteDialog->noteName;
	newNote->description = newNoteDialog->noteDescription;
	if(!newNoteDialog->fileName.isEmpty())
		newNote->fileName = newNoteDialog->fileName;
	newNote->parentType = "Accounts";
	newNote->parentId = acc->id;

	newNote->save();

	connect(newNote, SIGNAL(saved()),
			this, SLOT(displayNotes()));
}

void AccountDetail::progress(bool p)
{
	if(p) loading->show();
	else loading->hide();
}

void AccountDetail::hideButtons(const bool _var)
{
	if(_var) {
		newNote->hide();
		newDocument->hide();
		notesTable->hide();
	} else {
		newNote->show();
		newDocument->show();
		notesTable->show();
	}
}

void AccountDetail::showNewDocumentDialog()
{
	newNoteDialog->setUpload(true);
	newNoteDialog->show();
}

void AccountDetail::showNewNoteDialog()
{
	newNoteDialog->setUpload(false);
	newNoteDialog->show();
}

void AccountDetail::downloadNoteAttachment(const QModelIndex _index)
{
	if (acc->notes.at(_index.row())->fileName.isEmpty()) return;

	progress(true);
	connect(acc->notes.at(_index.row()), SIGNAL(openingAttachment()),
			this, SLOT(endProgress()));
	acc->notes.at(_index.row())->downloadAttachment();
}

void AccountDetail::endProgress()
{
	progress(false);
}
