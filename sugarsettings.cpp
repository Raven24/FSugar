#include "sugarsettings.h"

SugarSettings *SugarSettings::instance = NULL;

SugarSettings::SugarSettings(QObject *parent) :
	QObject(parent)
{
	m_settings = new QSettings(QSettings::IniFormat, QSettings::UserScope,
							   "FSugar", "SugarSoap", this);

	sugarHost = m_settings->value("SugarCrm/hostname", "aristoteles.serveftp.org").toString();
	sugarPath = m_settings->value("SugarCrm/path", "/Test/8/soap.php").toString();
	useSsl = m_settings->value("SugarCrm/useSsl", true).toBool();
}

SugarSettings* SugarSettings::getInstance()
{
	if(SugarSettings::instance == NULL) {
		SugarSettings::instance = new SugarSettings();
	}
	return SugarSettings::instance;
}
