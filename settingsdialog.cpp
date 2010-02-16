#include <QtGui>

#include "settingsdialog.h"
#include "sugarsettings.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent)
{
	settings = SugarSettings::getInstance();

	QVBoxLayout *layout = new QVBoxLayout();
	QGroupBox *sugarBox = new QGroupBox(tr("SugarCRM Konfiguration"));
	QFormLayout *form = new QFormLayout();
	QHBoxLayout *buttons = new QHBoxLayout();

	QLabel *header = new QLabel(tr("Einstellungen"));
	header->setProperty("heading", true);
	hostEdit = new QLineEdit(settings->sugarHost);
	pathEdit = new QLineEdit(settings->sugarPath);
	useSslEdit = new QCheckBox();
	useSslEdit->setChecked(settings->useSsl);
	QPushButton *saveBtn = new QPushButton(QIcon(":save.png"), tr("Speichern"));

	form->addRow(tr("Hostname"), hostEdit);
	form->addRow(tr("Pfad"), pathEdit);
	form->addRow(tr("Verschl\374sselung"), useSslEdit);
	sugarBox->setLayout(form);

	buttons->addStretch(5);
	buttons->addWidget(saveBtn);

	layout->addWidget(header);
	layout->addWidget(sugarBox);
	layout->addStretch(3);
	layout->addLayout(buttons);

	setLayout(layout);

	connect(saveBtn, SIGNAL(pressed()),
			this, SLOT(saveSettings()));
}

void SettingsDialog::saveSettings()
{
	settings->m_settings->setValue("SugarCrm/hostname", hostEdit->text());
	settings->m_settings->setValue("SugarCrm/path", pathEdit->text());
	settings->m_settings->setValue("SugarCrm/useSsl", useSslEdit->isChecked());
}
