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

#include <QtCore>
#include "cookiejar.h"
#include "mainwindow.h"

CookieJar::CookieJar(QObject* parent) :
		QNetworkCookieJar(parent)
{
	populateCookies();
	//qDebug() << "number of cookies read:" << allCookies().size();
}

CookieJar::~CookieJar()
{
	saveCookies();
}

void CookieJar::populateCookies()
{
	QList<QNetworkCookie> list;

	QDir::setCurrent(MainWindow::appPath); // make sure we are in the app's dir
	QFile f("cookies.txt");
	if(!f.open(QIODevice::ReadOnly)) {
		qDebug() << "[calendar] could not open cookies";
		return;
	}
	while(!f.atEnd()) {
		QList<QNetworkCookie> c = QNetworkCookie::parseCookies(f.readLine());
		list.append(c);
	}

	setAllCookies(list);
}

void CookieJar::saveCookies()
{
	//qDebug() << "saving cookies";

	QDir::setCurrent(MainWindow::appPath);
	QFile f("cookies.txt");
	qint64 bytesWritten = 0;

	if (!f.open(QIODevice::ReadWrite)) {
		qDebug() << "unable to save cookies";
		return;
	}

	QListIterator<QNetworkCookie> i(allCookies());
	while(i.hasNext()) {
		bytesWritten += f.write(i.next().toRawForm());
		bytesWritten += f.write("\n");
	}

	f.flush();
	f.resize(bytesWritten);
	f.close();
}
