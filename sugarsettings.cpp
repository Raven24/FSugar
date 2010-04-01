#include <QtGui>
#include "sugarsettings.h"
#include "mainwindow.h"

SugarSettings *SugarSettings::instance = NULL;

SugarSettings::SugarSettings(QObject *parent) :
	QObject(parent)
{
	QDir::setCurrent(MainWindow::appPath);
	m_settings = new QSettings("config.ini", QSettings::IniFormat, this);
	assignVars();
}

SugarSettings* SugarSettings::getInstance()
{
	if(SugarSettings::instance == NULL) {
		SugarSettings::instance = new SugarSettings();
	}
	return SugarSettings::instance;
}

void SugarSettings::assignVars()
{
	sugarHost = m_settings->value("SugarCrm/hostname", "www.example.com").toString();
	sugarPath = m_settings->value("SugarCrm/path", "/sugar/soap.php").toString();
	useSsl = m_settings->value("SugarCrm/useSsl", true).toBool();
	sugarUser = m_settings->value("SugarCrm/username", "User").toString();
	sugarPass = m_settings->value("SugarCrm/password", "pass").toString();

	calendarUrl = m_settings->value("Calendar/url", "http://calendar.google.com").toString();
}

void SugarSettings::makeUpdate()
{
	m_settings->sync();
	assignVars();
	emit settingsChanged();
}
