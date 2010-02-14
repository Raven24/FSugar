#include <QtGui>

#include "createnotedialog.h"

CreateNoteDialog::CreateNoteDialog(QWidget *parent)
	: QDialog(parent)
{
	name = new QLineEdit();

	description = new QTextEdit();
	QPushButton *save = new QPushButton(tr("Speichern"));
	QPushButton *cancel = new QPushButton(tr("Abbrechen"));
	QLabel *desc = new QLabel(tr("Text"));

	connect(save, SIGNAL(pressed()),
			this, SLOT(saveNote()));
	connect(cancel, SIGNAL(pressed()),
			this, SLOT(reject()));

	QFormLayout *form = new QFormLayout();
	form->addRow(tr("Betreff"), name);

	QHBoxLayout *buttons = new QHBoxLayout();
	buttons->addWidget(save);
	buttons->addWidget(cancel);

	QVBoxLayout *layout = new QVBoxLayout();
	layout->addLayout(form);
	layout->addWidget(desc);
	layout->addWidget(description);
	layout->addLayout(buttons);

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
	if (description->toPlainText().isEmpty()) {
		description->setProperty("mandatoryField", true);
		// styling doesn't work due to a bug...
		// ... so we need to set it manually
		description->setStyleSheet("border: 2px solid #B22222; border-radius: 2px; background-color: #eec0c0;");
		return;
	}

	noteName = name->text();
	noteDescription = description->toPlainText();

	name->clear();
	description->clear();

	accept();
}
