#include <QtGui>

#include "settingsdialog.h"
#include "sugarsettings.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent)
{
	settings = SugarSettings::getInstance();

	QVBoxLayout *layout = new QVBoxLayout();
	QGroupBox *sugarBox = new QGroupBox(tr("SugarCRM Konfiguration"));
	QGroupBox *calBox = new QGroupBox(tr("Kalender Konfiguration"));
	QFormLayout *sugarForm = new QFormLayout();
	QFormLayout *calForm = new QFormLayout();
	QHBoxLayout *buttons = new QHBoxLayout();

	QLabel *header = new QLabel(tr("Einstellungen"));
	header->setProperty("heading", true);
	hostEdit = new QLineEdit(settings->sugarHost);
	pathEdit = new QLineEdit(settings->sugarPath);
	calPathEdit = new QLineEdit(settings->calendarUrl);
	useSslEdit = new QCheckBox();
	useSslEdit->setChecked(settings->useSsl);
	QPushButton *saveBtn = new QPushButton(QIcon(":save.png"), tr("Speichern"));

	sugarForm->addRow(tr("Hostname"), hostEdit);
	sugarForm->addRow(tr("Pfad"), pathEdit);
	sugarForm->addRow(tr("Verschl\374sselung"), useSslEdit);
	sugarBox->setLayout(sugarForm);

	calForm->addRow(tr("Adresse"), calPathEdit);
	calBox->setLayout(calForm);

	buttons->addStretch(5);
	buttons->addWidget(saveBtn);

	layout->addWidget(header);
	layout->addWidget(sugarBox);
	layout->addWidget(calBox);
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

	settings->m_settings->setValue("Calendar/url", calPathEdit->text());

	settings->makeUpdate();
}
