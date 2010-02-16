#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QtGui>
#include "sugarsettings.h"

class SettingsDialog : public QDialog
{
Q_OBJECT
public:
	explicit SettingsDialog(QWidget *parent = 0);

signals:

public slots:
	void saveSettings();

private:
	SugarSettings *settings;

	QLineEdit *hostEdit, *pathEdit;
	QCheckBox *useSslEdit;

};

#endif // SETTINGSDIALOG_H
