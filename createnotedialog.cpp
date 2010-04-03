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

#include "createnotedialog.h"

CreateNoteDialog::CreateNoteDialog(QWidget *parent)
	: QDialog(parent)
{
	name = new QLineEdit();
	fileNameEdit = new QLineEdit();

	description = new QTextEdit();
	QPushButton *chooseFile = new QPushButton(tr("Auswahl..."));
	QPushButton *save = new QPushButton(tr("Speichern"));
	QPushButton *cancel = new QPushButton(tr("Abbrechen"));
	QLabel *desc = new QLabel(tr("Text"));

	connect(save, SIGNAL(pressed()),
			this, SLOT(saveNote()));
	connect(cancel, SIGNAL(pressed()),
			this, SLOT(reject()));
	connect(chooseFile, SIGNAL(pressed()),
			this, SLOT(showFileChooser()));

	QFormLayout *form = new QFormLayout();
	form->addRow(tr("Betreff"), name);

	QHBoxLayout *fileSelectLayout = new QHBoxLayout();
	fileSelectLayout->addWidget(fileNameEdit, 3);
	fileSelectLayout->addWidget(chooseFile, 2);
	QFormLayout *fileFormLayout = new QFormLayout();
	fileFormLayout->addRow(tr("Datei"), fileSelectLayout);
	fileLayout = new QWidget();
	fileLayout->setLayout(fileFormLayout);

	QHBoxLayout *buttons = new QHBoxLayout();
	buttons->addWidget(save);
	buttons->addWidget(cancel);

	QVBoxLayout *layout = new QVBoxLayout();
	layout->addLayout(form);
	layout->addWidget(fileLayout);
	layout->addWidget(desc);
	layout->addWidget(description);
	layout->addLayout(buttons);

	fileLayout->hide();
	setLayout(layout);
}

void CreateNoteDialog::saveNote()
{
	if (name->text().isEmpty()) {
		name->setProperty("mandatoryField", true);
		// styling doesn't work due to a bug...
		// ... so we need to set it manually
		name->setStyleSheet("border: 2px solid #B22222; border-radius: 2px; background-color: #eec0c0;");
		return;
	}
	name->setStyleSheet(""); name->setProperty("mandatoryField", false);
	if (description->toPlainText().isEmpty()) {
		description->setProperty("mandatoryField", true);
		// styling doesn't work due to a bug...
		// ... so we need to set it manually
		description->setStyleSheet("border: 2px solid #B22222; border-radius: 2px; background-color: #eec0c0;");
		return;
	}
	description->setStyleSheet(""); description->setProperty("mandatoryField", false);
	if (fileNameEdit->text().isEmpty() && fileNameEdit->isVisible()) {
		fileNameEdit->setProperty("mandatoryField", true);
		fileNameEdit->setStyleSheet("border: 2px solid #B22222; border-radius: 2px; background-color: #eec0c0;");
		return;
	}
	fileNameEdit->setStyleSheet(""); fileNameEdit->setProperty("mandatoryField", false);

	noteName = name->text();
	noteDescription = description->toPlainText();
	fileName = fileNameEdit->text();

	// reset the dialog
	name->clear();
	description->clear();
	fileNameEdit->clear();
	fileLayout->hide();

	accept();
}

void CreateNoteDialog::setUpload(const bool val)
{
	if(val) fileLayout->show();
	else fileLayout->hide();

}

void CreateNoteDialog::showFileChooser()
{
	fileName = QFileDialog::getOpenFileName(this,
											tr("Datei Waehlen"),
											QDesktopServices::storageLocation(QDesktopServices::HomeLocation)
											);
	fileNameEdit->setText(fileName);
}
