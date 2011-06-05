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
	QString user, pass, session, uid, noteAttachment;
	QList<QString> availableModules;
	QMap<QString, QMap<QString, QString> > *entries;
	QMap<QString, QMap<QString, QString> > notes;

signals:
	void sendingMessage(QString msg);
	void loginSuccessful();
	void loginFailed();
	void dataAvailable(QString id);
	void notesAvailable(QString id);
	void attachmentAvailable(QString id);
	void entryCreated(QString id);
	void entryUpdated(QString id);
	void returnedFaultyMessage(QString msg);
	void unknownAction(QString action);

public slots:
	void processResponse(const QtSoapMessage msg);
	void getUserInfo();
	void getServerTime();
	void getSugarFlavor();
	void getAvailableModules();

	void getAccountList();
	void getContactList();

	void getEntryList(const QString _module, const QString _query = "",
					  const QString _orderBy = "accounts.name ASC",
					  const int _offset = 0, const QString _selectFields = "",
					  const int _maxResults = 270,
					  const int _deleted = 0);

	void getRelatedNotes(const QString _module, const QString _id);
	void createNote(const QString _module = "Notes", const QString _name = "",
					const QString _description = "", const QString _parentType = "",
					const QString _parentId = "", const QString _contactId = "");
	void updateAccount(const QString _id, const QString _name, const QString _description,
					   const QString _addressStreet, const QString _addressCity,
					   const QString _addressPostalcode, const QString _addressCountry,
					   const QString _phoneOffice, const QString _phoneFax,
					   const QString _phoneAlternate, const QString _email,
					   const QString _website, const QString _category);
	void updateContact(const QString _id, const QString _firstName, const QString _lastName, const QString _description,
					   const QString _addressStreet, const QString _addressPostalcode, const QString _addressCity, const QString _addressCountry,
					   const QString _phoneWork, const QString _phoneHome, const QString _phoneMobile, const QString _phoneFax, const QString _phoneOther,
					   const QString _email1, const QString _email2,
					   const QString _accountId, const QString _accountName);
	void setNoteAttachment(const QString _id, const QString _filename, const QString _file);
	void getNoteAttachment(const QString _id);

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
