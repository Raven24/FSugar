#ifndef SUGARCRMSOAP_H
#define SUGARCRMSOAP_H

#include <QObject>
#include <qtsoap.h>

class SugarCrmSoap : public QObject
{
	Q_OBJECT

public:
	SugarCrmSoap(QString host);
	void setAction(const QString &action);

signals:
	void responseReady(const QtSoapMessage &response);
	void newSoapMessage(const QString msg);

public slots:
	void submitRequest(QtSoapMessage &request, const QString &path = "");

private slots:
	void debugSoapResponse(QtSoapMessage msg);
	void handleSslError(QNetworkReply* reply, const QList<QSslError> &errors);

private:
	QtSoapHttpTransport http;

};

#endif // SUGARCRMSOAP_H
