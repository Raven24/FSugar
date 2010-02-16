#include "sugarsettings.h"

SugarSettings *SugarSettings::instance = NULL;

SugarSettings::SugarSettings(QObject *parent) :
	QObject(parent)
{
	m_settings = new QSettings("config.ini", QSettings::IniFormat, this);

	sugarHost = m_settings->value("SugarCrm/hostname", "www.example.com").toString();
	sugarPath = m_settings->value("SugarCrm/path", "/sugar/soap.php").toString();
	useSsl = m_settings->value("SugarCrm/useSsl", true).toBool();
}

SugarSettings* SugarSettings::getInstance()
{
	if(SugarSettings::instance == NULL) {
		SugarSettings::instance = new SugarSettings();
	}
	return SugarSettings::instance;
}
