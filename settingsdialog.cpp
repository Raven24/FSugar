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
	QFormLayout *settingsForm = new QFormLayout();
	QFormLayout *calForm = new QFormLayout();
	QHBoxLayout *buttons = new QHBoxLayout();

	QLabel *header = new QLabel(tr("Einstellungen"));
	header->setProperty("heading", true);

	settingsSaved  = new QLabel(tr("Einstellungen gespeichert."));
	settingsSaved->hide();

	hostEdit     = new QLineEdit(settings->sugarHost);
	pathEdit     = new QLineEdit(settings->sugarPath);
	calPathEdit  = new QLineEdit(settings->calendarUrl);
	useSslEdit   = new QCheckBox();
	useSslEdit->setChecked(settings->useSsl);
	usernameEdit = new QLineEdit(settings->sugarUser);
	passwordEdit = new QLineEdit(settings->sugarPass);
	passwordEdit->setEchoMode(QLineEdit::Password);

	QPushButton *saveBtn = new QPushButton(QIcon(":save.png"), tr("Speichern"));

	settingsForm->addRow(tr("Hostname"), hostEdit);
	settingsForm->addRow(tr("Pfad"), pathEdit);
	settingsForm->addRow(tr("Verschl\374sselung"), useSslEdit);
	settingsForm->addRow(tr("Benutzername"), usernameEdit);
	settingsForm->addRow(tr("Passwort"), passwordEdit);
	sugarBox->setLayout(settingsForm);

	calForm->addRow(tr("Adresse"), calPathEdit);
	calBox->setLayout(calForm);

	buttons->addStretch(5);
	buttons->addWidget(settingsSaved);
	buttons->addWidget(saveBtn);

	layout->addWidget(header);

	QLabel *saveNote = new QLabel(tr("Um alle Aenderungen zu uebernehmen,\nmuss das Programm neu gestartet werden!"));
	saveNote->setAlignment(Qt::AlignCenter);
	layout->addWidget(saveNote, 1, Qt::AlignCenter);

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
		settingsSaved->show();

		QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(settingsSaved);
		settingsSaved->setGraphicsEffect(effect);
		QPropertyAnimation *anim = new QPropertyAnimation(effect, "opacity");
		anim->setStartValue(1.0);
		anim->setEndValue(0.01);
		anim->setDuration(2000);
		connect(anim, SIGNAL(finished()),
				settingsSaved, SLOT(hide()));

		anim->start();
		userChange = false;
	}
}
