#ifndef SUGARCRM_H
#define SUGARCRM_H

#include <QObject>

#include "sugarcrmsoap.h"
#include "sugarsettings.h"

class SugarCrm : public QObject
{
Q_OBJECT
public:
	SugarCrm(QObject *parent = 0);

	void login(QString user, QString pass);
	static SugarCrm *getInstance();

	SugarCrmSoap *trans;
	bool loggedin;
	QString user, pass, session, uid;
	QList<QString> availableModules;
	QMap<QString, QMap<QString, QString> > entries;
	QMap<QString, QMap<QString, QString> > notes;

signals:
	void sendingMessage(QString msg);
	void loginSuccessful();
	void loginFailed();
	void dataAvailable();
	void notesAvailable();
	void unknownAction(QString action);

public slots:
	void processResponse(const QtSoapMessage msg);
	void getUserInfo();
	void getServerTime();
	void getSugarFlavor();
	void getAvailableModules();
	void getEntryList(const QString _module, const QString _query = "",
					  const QString _orderBy = "accounts.name ASC",
					  const int _offset = 0, const int _maxResults = 100,
					  const int _deleted = 0);
	void getRelatedNotes(const QString _module, const QString _id);

private:
	void submit(QtSoapMessage msg, QString action);
	QtSoapMessage createMessage(QString method);
	void decideAction(const QString action, const QMap<QString, QString> data);
	void decideAction(const QString action, const QtSoapStruct data);

	static SugarCrm *instance;

	QDateTime serverTime;
	SugarSettings *settings;
};

#endif // SUGARCRM_H
