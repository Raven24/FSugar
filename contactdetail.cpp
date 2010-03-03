#include "abstractitemdetail.h"
#include "contactdetail.h"

ContactDetail::ContactDetail(QWidget *parent) :
	AbstractItemDetail(parent)
{
}

void ContactDetail::initDialog()
{
	AbstractItemDetail::initDialog();

	// layouts
	QFormLayout *addressLayout = new QFormLayout();
	QFormLayout *phoneLayout = new QFormLayout();
	QHBoxLayout *nameLayout = new QHBoxLayout();
	QHBoxLayout *itemsContainer = new QHBoxLayout();
	QHBoxLayout *contactInfoLayout = new QHBoxLayout();
	QVBoxLayout *layout = new QVBoxLayout();

	// edit fields
	firstNameEdit = new QLineEdit();
	lastNameEdit = new QLineEdit();
	phoneHomeEdit = new QLineEdit();
	phoneMobileEdit = new QLineEdit();
	phoneWorkEdit = new QLineEdit();
	phoneOtherEdit = new QLineEdit();
	phoneFaxEdit = new QLineEdit();
	email1Edit = new QLineEdit();
	email2Edit = new QLineEdit();
	addressStreetEdit = new QLineEdit();
	addressCityEdit = new QLineEdit();
	addressPostalcodeEdit = new QLineEdit();
	addressCountryEdit = new QLineEdit();
	leadSourceEdit = new QLineEdit();
	contactDescriptionEdit = new QTextEdit();

	QFont f = firstNameEdit->font();
	f.setPixelSize(14);
	firstNameEdit->setFont(f);
	f.setBold(true);
	lastNameEdit->setFont(f);

	QLabel *descriptionLabel = new QLabel(tr("Beschreibung"));

	nameLayout->addWidget(lastNameEdit);
	nameLayout->addWidget(firstNameEdit);

	addressLayout->setLabelAlignment(Qt::AlignRight);
	addressLayout->addRow(tr("Adresse"), addressStreetEdit);
	addressLayout->addRow(tr("PLZ"), addressPostalcodeEdit);
	addressLayout->addRow(tr("Ort"), addressCityEdit);
	addressLayout->addRow(tr("Land"), addressCountryEdit);

	addressLayout->addRow(tr("Email"), email1Edit);
	addressLayout->addRow(tr("Email 2"), email2Edit);

	phoneLayout->addRow(tr("Telefon Arbeit"), phoneWorkEdit);
	phoneLayout->addRow(tr("Mobil"), phoneMobileEdit);
	phoneLayout->addRow(tr("Fax"), phoneFaxEdit);
	phoneLayout->addRow(tr("Telefon Zuhause"), phoneHomeEdit);
	phoneLayout->addRow(tr("Telefon weiteres"), phoneOtherEdit);

	contactInfoLayout->addLayout(addressLayout);
	contactInfoLayout->addLayout(phoneLayout);

	itemsContainer->addWidget(save, 0, Qt::AlignLeft);
	itemsContainer->addStretch(3);
	itemsContainer->addWidget(loading, 0, Qt::AlignCenter);
	itemsContainer->addStretch(3);
	itemsContainer->addWidget(newNote, 0, Qt::AlignRight);
	itemsContainer->addWidget(newDocument, 0, Qt::AlignRight);

	layout->addLayout(nameLayout);
	layout->addLayout(contactInfoLayout);
	layout->addWidget(descriptionLabel, 0, Qt::AlignTop);
	layout->addWidget(contactDescriptionEdit, 1, Qt::AlignTop);
	layout->addStretch(3);
	layout->addLayout(itemsContainer);
	layout->addWidget(notesTable);

	setLayout(layout);
}

void ContactDetail::saveChanges()
{
	progress(true);

	getItem()->firstName = firstNameEdit->text();
	getItem()->lastName = lastNameEdit->text();
	getItem()->phoneHome = phoneHomeEdit->text();
	getItem()->phoneMobile = phoneMobileEdit->text();
	getItem()->phoneWork = phoneWorkEdit->text();
	getItem()->phoneOther = phoneOtherEdit->text();
	getItem()->phoneFax = phoneFaxEdit->text();
	getItem()->email1 = email1Edit->text();
	getItem()->email2 = email2Edit->text();
	getItem()->addressCity = addressCityEdit->text();
	getItem()->addressCountry = addressCountryEdit->text();
	getItem()->addressPostalcode = addressPostalcodeEdit->text();
	getItem()->addressStreet = addressStreetEdit->text();
	getItem()->description = contactDescriptionEdit->toPlainText();

	getItem()->save();
}

void ContactDetail::retrieveContact(Contact* _c)
{
	item = _c;
	initDialog();
	fillData();

	connect(crm, SIGNAL(entryCreated(QString)),
			getItem(), SLOT(getNotes()));
	connect(getItem(), SIGNAL(saved()),
			this, SLOT(afterSaveAct()));
	connect(getItem(), SIGNAL(notesAvailable()),
			this, SLOT(displayNotes()));

	getItem()->getChildren();
}

Contact* ContactDetail::getItem()
{
	return item;
}

void ContactDetail::fillData()
{
	firstNameEdit->setText(getItem()->firstName);
	lastNameEdit->setText(getItem()->lastName);
	phoneHomeEdit->setText(getItem()->phoneHome);
	phoneMobileEdit->setText(getItem()->phoneMobile);
	phoneWorkEdit->setText(getItem()->phoneWork);
	phoneOtherEdit->setText(getItem()->phoneOther);
	phoneFaxEdit->setText(getItem()->phoneFax);
	email1Edit->setText(getItem()->email1);
	email2Edit->setText(getItem()->email2);
	addressStreetEdit->setText(getItem()->addressStreet);
	addressCityEdit->setText(getItem()->addressCity);
	addressPostalcodeEdit->setText(getItem()->addressPostalcode);
	addressCountryEdit->setText(getItem()->addressCountry);
	contactDescriptionEdit->setText(getItem()->description);
}
