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

#include "browserwidget.h"

BrowserWidget::BrowserWidget(QWidget *parent) :
    QWidget(parent)
{
	QNetworkAccessManager *nwManager = new QNetworkAccessManager();
	cookieJar = CookieJar::getInstance();
	nwManager->setCookieJar(cookieJar);
	webView = new QWebView(this);
	webView->page()->setNetworkAccessManager(nwManager);

	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(webView);

	connect(nwManager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
			this, SLOT(handleSslError(QNetworkReply*,QList<QSslError>)));

	setLayout(layout);
}

BrowserWidget::~BrowserWidget()
{
	delete cookieJar;
}

void BrowserWidget::setAddress(QUrl link)
{
	webView->load(link);
}

void BrowserWidget::handleSslError(QNetworkReply *reply, const QList<QSslError> &errors)
{
	Q_UNUSED(errors)

	qDebug() << "[ssl] server host: " << reply->sslConfiguration().peerCertificate().issuerInfo(QSslCertificate::CommonName);
	reply->ignoreSslErrors();
}
