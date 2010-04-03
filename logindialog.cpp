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
