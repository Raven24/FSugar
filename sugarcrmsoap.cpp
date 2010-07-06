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
#include <QtNetwork>

#include "sugarcrmsoap.h"
#include "sugarsettings.h"

SugarCrmSoap::SugarCrmSoap()
{
	settings = SugarSettings::getInstance();
	setHost();

	connect(settings, SIGNAL(settingsChanged()),
			this, SLOT(setHost()));

	connect(&http, SIGNAL(responseReady(QtSoapMessage)),
			this, SIGNAL(responseReady(QtSoapMessage)));
	connect(this, SIGNAL(responseReady(QtSoapMessage)),
			this, SLOT(debugSoapResponse(QtSoapMessage)));
	connect(http.networkAccessManager(), SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
			this, SLOT(handleSslError(QNetworkReply*, QList<QSslError>)));

}

void SugarCrmSoap::setHost()
{
	qDebug() << "[soap] setting host to " << settings->sugarHost;
	http.setHost(settings->sugarHost, settings->useSsl );
}

void SugarCrmSoap::setAction(const QString &action)
{
	http.setAction(action);
}

void SugarCrmSoap::submitRequest(QtSoapMessage &request)
{
	qDebug() << "[soap] sending request, path: " << settings->sugarPath;
	http.submitRequest(request, settings->sugarPath);
}

void SugarCrmSoap::debugSoapResponse(QtSoapMessage msg)
{
	emit newSoapMessage(msg.toXmlString());
}

void SugarCrmSoap::handleSslError(QNetworkReply* reply, const QList<QSslError> &errors)
{
	Q_UNUSED(errors)

	qDebug() << "[ssl] server host: " << reply->sslConfiguration().peerCertificate().issuerInfo(QSslCertificate::CommonName);
	reply->ignoreSslErrors();
}
