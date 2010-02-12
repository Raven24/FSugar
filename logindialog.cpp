#include <QtGui>
#include <QDialog>
#include "logindialog.h"
#include "sugarcrm.h"

LoginDialog::LoginDialog(QWidget *parent)
	: QDialog(parent)

{
	user = new QLineEdit;
	pass = new QLineEdit;
	pass->setEchoMode(QLineEdit::Password);

	ok = new QPushButton(tr("OK"));
	ok->setDefault(true);

	abort = new QPushButton(tr("Abbrechen"));

	QFormLayout *form = new QFormLayout;
	form->addRow(tr("Benutzername"), user);
	form->addRow(tr("Passwort"), pass);

	QHBoxLayout *buttons = new QHBoxLayout;
	buttons->addWidget(ok);
	buttons->addWidget(abort);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addLayout(form);
	layout->addLayout(buttons);

	connect(abort, SIGNAL(pressed()),
			this, SLOT(reject()));
	connect(ok, SIGNAL(pressed()),
			this, SLOT(accept()));


	setLayout(layout);
}

LoginDialog::~LoginDialog()
{
}

QString LoginDialog::getUsername()
{
	return QString(user->text());
}

QString LoginDialog::getPassword()
{
	return QString(pass->text());
}
