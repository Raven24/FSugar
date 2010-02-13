#include <QtGui>

#include "createnotedialog.h"

CreateNoteDialog::CreateNoteDialog(QWidget *parent)
	: QDialog(parent)
{
	name = new QLineEdit();
	description = new QTextEdit();
	QPushButton *save = new QPushButton(tr("Speichern"));
	QPushButton *cancel = new QPushButton(tr("Abbrechen"));
	QLabel *desc = new QLabel(tr("Beschreibung"));

	connect(save, SIGNAL(pressed()),
			this, SLOT(saveNote()));
	connect(cancel, SIGNAL(pressed()),
			this, SLOT(reject()));

	QFormLayout *form = new QFormLayout();
	form->addRow(tr("Name"), name);

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
	noteName = name->text();
	noteDescription = description->toPlainText();

	name->clear();
	description->clear();

	accept();
}
