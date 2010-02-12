#include <QtCore>
#include <QtNetwork>

#include "sugarcrmsoap.h"
#include "sugarsettings.h"

SugarCrmSoap::SugarCrmSoap(QString host)
{
	http.setHost(host, SugarSettings::getInstance()->useSsl );

	connect(&http, SIGNAL(responseReady(QtSoapMessage)),
			this, SIGNAL(responseReady(QtSoapMessage)));
	connect(this, SIGNAL(responseReady(QtSoapMessage)),
			this, SLOT(debugSoapResponse(QtSoapMessage)));
	connect(http.networkAccessManager(), SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
			this, SLOT(handleSslError(QNetworkReply*, QList<QSslError>)));

}

void SugarCrmSoap::setAction(const QString &action)
{
	http.setAction(action);
}

void SugarCrmSoap::submitRequest(QtSoapMessage &request, const QString &path)
{
	http.submitRequest(request, path);
}

void SugarCrmSoap::debugSoapResponse(QtSoapMessage msg)
{
	emit newSoapMessage(msg.toXmlString());
}

void SugarCrmSoap::handleSslError(QNetworkReply* reply, const QList<QSslError> &errors)
{
	Q_UNUSED(errors)

	qDebug() << reply->sslConfiguration().peerCertificate().issuerInfo(QSslCertificate::CommonName);
	reply->ignoreSslErrors();
}
