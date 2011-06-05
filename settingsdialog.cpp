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

#include "settingsdialog.h"
#include "sugarsettings.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent)
{
	settings = SugarSettings::getInstance();
	userChange = false;

	QVBoxLayout *layout = new QVBoxLayout();
	QGroupBox *sugarBox = new QGroupBox(tr("SugarCRM Konfiguration"));
	QGroupBox *calBox = new QGroupBox(tr("Kalender Konfiguration"));
	QFormLayout *sugarForm = new QFormLayout();
	QFormLayout *calForm = new QFormLayout();
	QHBoxLayout *buttons = new QHBoxLayout();

	QLabel *header = new QLabel(tr("Einstellungen"));
	header->setProperty("heading", true);
	hostEdit     = new QLineEdit(settings->sugarHost);
	pathEdit     = new QLineEdit(settings->sugarPath);
	calPathEdit  = new QLineEdit(settings->calendarUrl);
	useSslEdit   = new QCheckBox();
	useSslEdit->setChecked(settings->useSsl);
	usernameEdit = new QLineEdit(settings->sugarUser);
	passwordEdit = new QLineEdit(settings->sugarPass);
	passwordEdit->setEchoMode(QLineEdit::Password);

	QPushButton *saveBtn = new QPushButton(QIcon(":save.png"), tr("Speichern"));

	sugarForm->addRow(tr("Hostname"), hostEdit);
	sugarForm->addRow(tr("Pfad"), pathEdit);
	sugarForm->addRow(tr("Verschl\374sselung"), useSslEdit);
	sugarForm->addRow(tr("Benutzername"), usernameEdit);
	sugarForm->addRow(tr("Passwort"), passwordEdit);
	sugarBox->setLayout(sugarForm);

	calForm->addRow(tr("Adresse"), calPathEdit);
	calBox->setLayout(calForm);

	buttons->addStretch(5);
	buttons->addWidget(saveBtn);

	layout->addWidget(header);

	layout->addWidget(new QLabel(tr("Um die Aenderungen zu uebernehmen, muss das Programm neu gestartet werden!")), 1, Qt::AlignCenter);

	layout->addWidget(sugarBox);

	layout->addWidget(calBox);
	layout->addStretch(3);
	layout->addLayout(buttons);

	setLayout(layout);

	connect(saveBtn, SIGNAL(pressed()),
			this, SLOT(saveSettings()));
	connect(settings, SIGNAL(settingsChanged()),
			this, SLOT(showSettingsUpdated()));
}

void SettingsDialog::saveSettings()
{
	userChange = true;
	qDebug() << "[settings] saving settings";

	settings->m_settings->setValue("SugarCrm/hostname", hostEdit->text());
	settings->m_settings->setValue("SugarCrm/path", pathEdit->text());
	settings->m_settings->setValue("SugarCrm/useSsl", useSslEdit->isChecked());
	settings->m_settings->setValue("SugarCrm/username", usernameEdit->text());

	// TODO: "encrypt" password
	// http://lists.trolltech.com/qt-interest/2007-11/thread00001-0.html
	settings->m_settings->setValue("SugarCrm/password", passwordEdit->text());

	settings->m_settings->setValue("Calendar/url", calPathEdit->text());

	settings->makeUpdate();
}

void SettingsDialog::showSettingsUpdated()
{
	if( userChange ) {
		QMessageBox msgBox;
		msgBox.setText(tr("The settings have been updated..."));
		msgBox.exec();
		userChange = false;
	}
}
