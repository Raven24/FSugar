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

#ifndef SUGARCRMSOAP_H
#define SUGARCRMSOAP_H

#include <QObject>
#include <qtsoap.h>
#include <sugarsettings.h>

class SugarCrmSoap : public QObject
{
	Q_OBJECT

public:
	SugarCrmSoap();
	void setAction(const QString &action);

signals:
	void responseReady(const QtSoapMessage &response);
	void newSoapMessage(const QString msg);

public slots:
	void submitRequest(QtSoapMessage &request);

private slots:
	void debugSoapResponse(QtSoapMessage msg);
	void handleSslError(QNetworkReply* reply, const QList<QSslError> &errors);
	void setHost();

private:
	QtSoapHttpTransport http;
	SugarSettings *settings;

};

#endif // SUGARCRMSOAP_H
