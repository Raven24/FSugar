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

	windowGeometry = m_settings->value("Application/geometry").toByteArray();
}

void SugarSettings::makeUpdate()
{
	m_settings->sync();
	assignVars();
	emit settingsChanged();
}
