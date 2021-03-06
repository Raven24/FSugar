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

#include "qtsoap.h"
#include "sugarcrmsoap.h"
#include "sugarcrm.h"
#include "sugarsettings.h"

SugarCrm *SugarCrm::instance = NULL;

SugarCrm::SugarCrm(QObject *parent) :
    QObject(parent)
{
	settings = new SugarSettings();
	trans = new SugarCrmSoap();

	loggedin = false;

	connect(trans, SIGNAL(responseReady(QtSoapMessage)),
			this, SLOT(processResponse(QtSoapMessage)));
	//connect(this, SIGNAL(loginSuccessful()),
	//		this, SLOT(getUserInfo()));

}

void SugarCrm::login(QString _user, QString _pass)
{
	user = _user;
	pass = _pass;

	QtSoapStruct *user_auth = new QtSoapStruct(QtSoapQName("user_auth"));
	user_auth->insert(new QtSoapSimpleType(QtSoapQName("user_name"), user));
	user_auth->insert(new QtSoapSimpleType(QtSoapQName("password"), pass));
	user_auth->insert(new QtSoapSimpleType(QtSoapQName("version"), "0.1"));

	QtSoapMessage msg = createMessage("login");
	//msg.addMethodArgument("user_name", "", user);
	//msg.addMethodArgument("password", "", pass);
	msg.addMethodArgument(user_auth);
	msg.addMethodArgument("application_name", "", "SugarCrmTest");

	submit(msg, "login");
}

void SugarCrm::createNote(const QString _module, const QString _name, const QString _description, const QString _parentType, const QString _parentId, const QString _contactId)
{
	QtSoapStruct *name = new QtSoapStruct(QtSoapQName("name_value"));
	name->insert(new QtSoapSimpleType(QtSoapQName("name"), "name"));
	name->insert(new QtSoapSimpleType(QtSoapQName("value"), _name));

	QtSoapStruct *desc = new QtSoapStruct(QtSoapQName("name_value"));
	desc->insert(new QtSoapSimpleType(QtSoapQName("name"), "description"));
	desc->insert(new QtSoapSimpleType(QtSoapQName("value"), _description));

	QtSoapStruct *parType = new QtSoapStruct(QtSoapQName("name_value"));
	parType->insert(new QtSoapSimpleType(QtSoapQName("name"), "parent_type"));
	parType->insert(new QtSoapSimpleType(QtSoapQName("value"), _parentType));

	QtSoapStruct *parId = new QtSoapStruct(QtSoapQName("name_value"));
	parId->insert(new QtSoapSimpleType(QtSoapQName("name"), "parent_id"));
	parId->insert(new QtSoapSimpleType(QtSoapQName("value"), _parentId));

	QtSoapStruct *contId = new QtSoapStruct(QtSoapQName("name_value"));
	contId->insert(new QtSoapSimpleType(QtSoapQName("name"), "contact_id"));
	contId->insert(new QtSoapSimpleType(QtSoapQName("value"), _contactId));

	QtSoapArray *note = new QtSoapArray(QtSoapQName("name_value_list"));
	note->insert(0, name);
	note->insert(1, desc);
	note->insert(2, parType);
	note->insert(3, parId);
	note->insert(4, contId);

	QtSoapMessage msg = createMessage("set_entry");
	msg.addMethodArgument("session", "", session);
	msg.addMethodArgument("module_name", "", _module);
	msg.addMethodArgument(note);
	submit(msg, "set_entry");
}

void SugarCrm::setNoteAttachment(const QString _id, const QString _filename, const QString _file)
{
	QtSoapStruct *noteAttachment = new QtSoapStruct(QtSoapQName("note_attachment"));
	noteAttachment->insert(new QtSoapSimpleType(QtSoapQName("id"), _id ));
	noteAttachment->insert(new QtSoapSimpleType(QtSoapQName("filename"), _filename));
	noteAttachment->insert(new QtSoapSimpleType(QtSoapQName("file"), _file));

	QtSoapMessage msg = createMessage("set_note_attachment");
	msg.addMethodArgument("session", "", session);
	msg.addMethodArgument(noteAttachment);
	submit(msg, "set_note_attachment");
}

void SugarCrm::getNoteAttachment(const QString _id)
{
	noteAttachment.clear();

	QtSoapMessage msg = createMessage("get_note_attachment");
	msg.addMethodArgument("session", "", session);
	msg.addMethodArgument("id", "", _id);
	submit(msg, "get_note_attachment");
}

SugarCrm* SugarCrm::getInstance()
{
	if(SugarCrm::instance == NULL) {
		SugarCrm::instance = new SugarCrm();
	}
	return SugarCrm::instance;
}

void SugarCrm::getServerTime()
{
	submit(createMessage("get_server_time"), "get_server_time");
}

void SugarCrm::getSugarFlavor()
{
	submit(createMessage("get_sugar_flavor"), "get_sugar_flavor");
}

void SugarCrm::getAvailableModules()
{
	QtSoapMessage msg = createMessage("get_available_modules");
	msg.addMethodArgument("session", "", session);
	submit(msg, "get_available_modules");
}

void SugarCrm::getAccountList()
{
	getEntryList("Accounts",		  // module
				 "",				  // query
				 "accounts.name ASC", // order
				 0,  				  // offset
				 "accounts.name",	  // select fields
				 500,				  // max results
				 0					  // deleted
	);
}

void SugarCrm::getContactList()
{
	getEntryList("Contacts",				// module
				 "",						// query
				 "contacts.last_name ASC",	// order
				 0,							// offset
				 "contacts.last_name,contacts.first_name", // select fields
				 500,						// max results
				 0							// deleted
	);
}

void SugarCrm::getEntryList(const QString _module, const QString _query,
							const QString _orderBy, const int _offset,
							const QString _selectFields,
							const int _maxResults, const int _deleted)
{
	entries = new QMap<QString, QMap<QString, QString> > ();

	QtSoapMessage msg = createMessage("get_entry_list");
	msg.addMethodArgument("session",	 "",   session);
	msg.addMethodArgument("module_name", "",   _module );
	msg.addMethodArgument("query",		 "",   _query);
	msg.addMethodArgument("order_by",	 "",   _orderBy);
	msg.addMethodArgument("offset",		 "",   _offset);
	msg.addMethodArgument("select_fields", "", _selectFields);
	msg.addMethodArgument("max_results", "",   _maxResults);
	msg.addMethodArgument("deleted",	 "",   _deleted);

	submit(msg, "get_entry_list");
}

void SugarCrm::getRelatedNotes(const QString _module, const QString _id)
{
	notes.clear();

	//qDebug() << _id << _module << "wants notes";

	QtSoapMessage msg = createMessage("get_related_notes");
	msg.addMethodArgument("session", "", session);
	msg.addMethodArgument("module_name", "", _module);
	msg.addMethodArgument("module_id", "", _id);
	submit(msg, "get_related_notes");
}

void SugarCrm::updateContact(const QString _id, const QString _firstName, const QString _lastName, const QString _description,
							 const QString _addressStreet, const QString _addressPostalcode, const QString _addressCity, const QString _addressCountry,
							 const QString _phoneWork, const QString _phoneHome, const QString _phoneMobile, const QString _phoneFax, const QString _phoneOther,
							 const QString _email1, const QString _email2,
							 const QString _accountId, const QString _accountName)
{
	QtSoapStruct *id = new QtSoapStruct(QtSoapQName("name_value"));
	id->insert(new QtSoapSimpleType(QtSoapQName("name"), "id"));
	id->insert(new QtSoapSimpleType(QtSoapQName("value"), _id));

	QtSoapStruct *firstName = new QtSoapStruct(QtSoapQName("name_value"));
	firstName->insert(new QtSoapSimpleType(QtSoapQName("name"), "first_name"));
	firstName->insert(new QtSoapSimpleType(QtSoapQName("value"), _firstName));

	QtSoapStruct *lastName = new QtSoapStruct(QtSoapQName("name_value"));
	lastName->insert(new QtSoapSimpleType(QtSoapQName("name"), "last_name"));
	lastName->insert(new QtSoapSimpleType(QtSoapQName("value"), _lastName));

	QtSoapStruct *desc = new QtSoapStruct(QtSoapQName("name_value"));
	desc->insert(new QtSoapSimpleType(QtSoapQName("name"), "description"));
	desc->insert(new QtSoapSimpleType(QtSoapQName("value"), _description));

	QtSoapStruct *addrSt = new QtSoapStruct(QtSoapQName("name_value"));
	addrSt->insert(new QtSoapSimpleType(QtSoapQName("name"), "primary_address_street"));
	addrSt->insert(new QtSoapSimpleType(QtSoapQName("value"), _addressStreet));

	QtSoapStruct *addrCi = new QtSoapStruct(QtSoapQName("name_value"));
	addrCi->insert(new QtSoapSimpleType(QtSoapQName("name"), "primary_address_city"));
	addrCi->insert(new QtSoapSimpleType(QtSoapQName("value"), _addressCity));

	QtSoapStruct *addrPo = new QtSoapStruct(QtSoapQName("name_value"));
	addrPo->insert(new QtSoapSimpleType(QtSoapQName("name"), "primary_address_postalcode"));
	addrPo->insert(new QtSoapSimpleType(QtSoapQName("value"), _addressPostalcode));

	QtSoapStruct *addrCo = new QtSoapStruct(QtSoapQName("name_value"));
	addrCo->insert(new QtSoapSimpleType(QtSoapQName("name"), "primary_address_country"));
	addrCo->insert(new QtSoapSimpleType(QtSoapQName("value"), _addressCountry));

	QtSoapStruct *phW = new QtSoapStruct(QtSoapQName("name_value"));
	phW->insert(new QtSoapSimpleType(QtSoapQName("name"), "phone_work"));
	phW->insert(new QtSoapSimpleType(QtSoapQName("value"), _phoneWork));

	QtSoapStruct *phF = new QtSoapStruct(QtSoapQName("name_value"));
	phF->insert(new QtSoapSimpleType(QtSoapQName("name"), "phone_fax"));
	phF->insert(new QtSoapSimpleType(QtSoapQName("value"), _phoneFax));

	QtSoapStruct *phH = new QtSoapStruct(QtSoapQName("name_value"));
	phH->insert(new QtSoapSimpleType(QtSoapQName("name"), "phone_home"));
	phH->insert(new QtSoapSimpleType(QtSoapQName("value"), _phoneHome));

	QtSoapStruct *phM = new QtSoapStruct(QtSoapQName("name_value"));
	phM->insert(new QtSoapSimpleType(QtSoapQName("name"), "phone_mobile"));
	phM->insert(new QtSoapSimpleType(QtSoapQName("value"), _phoneMobile));

	QtSoapStruct *phO = new QtSoapStruct(QtSoapQName("name_value"));
	phO->insert(new QtSoapSimpleType(QtSoapQName("name"), "phone_other"));
	phO->insert(new QtSoapSimpleType(QtSoapQName("value"), _phoneOther));

	QtSoapStruct *email = new QtSoapStruct(QtSoapQName("name_value"));
	email->insert(new QtSoapSimpleType(QtSoapQName("name"), "email1"));
	email->insert(new QtSoapSimpleType(QtSoapQName("value"), _email1));

	QtSoapStruct *email2 = new QtSoapStruct(QtSoapQName("name_value"));
	email2->insert(new QtSoapSimpleType(QtSoapQName("name"), "email2"));
	email2->insert(new QtSoapSimpleType(QtSoapQName("value"), _email2));

	QtSoapStruct *accId = new QtSoapStruct(QtSoapQName("name_value"));
	accId->insert(new QtSoapSimpleType(QtSoapQName("name"), "account_id"));
	accId->insert(new QtSoapSimpleType(QtSoapQName("value"), _accountId));

	QtSoapStruct *accName = new QtSoapStruct(QtSoapQName("name_value"));
	accName->insert(new QtSoapSimpleType(QtSoapQName("name"), "account_name"));
	accName->insert(new QtSoapSimpleType(QtSoapQName("value"), _accountName));

	QtSoapArray *contact = new QtSoapArray(QtSoapQName("name_value_list"));
	contact->insert(0, id);
	contact->insert(1, firstName);
	contact->insert(2, lastName);
	contact->insert(3, desc);
	contact->insert(4, addrCi);
	contact->insert(5, addrPo);
	contact->insert(6, addrCo);
	contact->insert(7, addrSt);
	contact->insert(8, phW);
	contact->insert(9, phF);
	contact->insert(10, phH);
	contact->insert(11, phM);
	contact->insert(12, phO);
	contact->insert(13, email);
	contact->insert(14, email2);
	contact->insert(15, accId);
	contact->insert(16, accName);

	QtSoapMessage msg = createMessage("set_entry");
	msg.addMethodArgument("session", "", session);
	msg.addMethodArgument("module_name", "", "Contacts");
	msg.addMethodArgument(contact);
	submit(msg, "set_entry");

}

void SugarCrm::updateAccount(const QString _id, const QString _name, const QString _description,
							 const QString _addressStreet, const QString _addressCity, const QString _addressPostalcode,
							 const QString _addressCountry, const QString _phoneOffice, const QString _phoneFax,
							 const QString _phoneAlternate, const QString _email, const QString _website, const QString _category)
{
	QtSoapStruct *id = new QtSoapStruct(QtSoapQName("name_value"));
	id->insert(new QtSoapSimpleType(QtSoapQName("name"), "id"));
	id->insert(new QtSoapSimpleType(QtSoapQName("value"), _id));

	QtSoapStruct *name = new QtSoapStruct(QtSoapQName("name_value"));
	name->insert(new QtSoapSimpleType(QtSoapQName("name"), "name"));
	name->insert(new QtSoapSimpleType(QtSoapQName("value"), _name));

	QtSoapStruct *desc = new QtSoapStruct(QtSoapQName("name_value"));
	desc->insert(new QtSoapSimpleType(QtSoapQName("name"), "description"));
	desc->insert(new QtSoapSimpleType(QtSoapQName("value"), _description));

	QtSoapStruct *addrSt = new QtSoapStruct(QtSoapQName("name_value"));
	addrSt->insert(new QtSoapSimpleType(QtSoapQName("name"), "billing_address_street"));
	addrSt->insert(new QtSoapSimpleType(QtSoapQName("value"), _addressStreet));

	QtSoapStruct *addrCi = new QtSoapStruct(QtSoapQName("name_value"));
	addrCi->insert(new QtSoapSimpleType(QtSoapQName("name"), "billing_address_city"));
	addrCi->insert(new QtSoapSimpleType(QtSoapQName("value"), _addressCity));

	QtSoapStruct *addrPo = new QtSoapStruct(QtSoapQName("name_value"));
	addrPo->insert(new QtSoapSimpleType(QtSoapQName("name"), "billing_address_postalcode"));
	addrPo->insert(new QtSoapSimpleType(QtSoapQName("value"), _addressPostalcode));

	QtSoapStruct *addrCo = new QtSoapStruct(QtSoapQName("name_value"));
	addrCo->insert(new QtSoapSimpleType(QtSoapQName("name"), "billing_address_country"));
	addrCo->insert(new QtSoapSimpleType(QtSoapQName("value"), _addressCountry));

	QtSoapStruct *phO = new QtSoapStruct(QtSoapQName("name_value"));
	phO->insert(new QtSoapSimpleType(QtSoapQName("name"), "phone_office"));
	phO->insert(new QtSoapSimpleType(QtSoapQName("value"), _phoneOffice));

	QtSoapStruct *phF = new QtSoapStruct(QtSoapQName("name_value"));
	phF->insert(new QtSoapSimpleType(QtSoapQName("name"), "phone_fax"));
	phF->insert(new QtSoapSimpleType(QtSoapQName("value"), _phoneFax));

	QtSoapStruct *phA = new QtSoapStruct(QtSoapQName("name_value"));
	phA->insert(new QtSoapSimpleType(QtSoapQName("name"), "phone_alternate"));
	phA->insert(new QtSoapSimpleType(QtSoapQName("value"), _phoneAlternate));

	QtSoapStruct *email = new QtSoapStruct(QtSoapQName("name_value"));
	email->insert(new QtSoapSimpleType(QtSoapQName("name"), "email1"));
	email->insert(new QtSoapSimpleType(QtSoapQName("value"), _email));

	QtSoapStruct *web = new QtSoapStruct(QtSoapQName("name_value"));
	web->insert(new QtSoapSimpleType(QtSoapQName("name"), "website"));
	web->insert(new QtSoapSimpleType(QtSoapQName("value"), _website));

	QtSoapStruct *cat = new QtSoapStruct(QtSoapQName("name_value"));
	cat->insert(new QtSoapSimpleType(QtSoapQName("name"), "kategorie_c"));
	cat->insert(new QtSoapSimpleType(QtSoapQName("value"), _category));

	QtSoapArray *account = new QtSoapArray(QtSoapQName("name_value_list"));
	account->insert(0, id);
	account->insert(1, name);
	account->insert(2, desc);
	account->insert(3, addrSt);
	account->insert(4, addrCi);
	account->insert(5, addrPo);
	account->insert(6, addrCo);
	account->insert(7, phO);
	account->insert(8, phF);
	account->insert(9, phA);
	account->insert(10, email);
	account->insert(11, web);
	account->insert(12, cat);

	QtSoapMessage msg = createMessage("set_entry");
	msg.addMethodArgument("session", "", session);
	msg.addMethodArgument("module_name", "", "Accounts");
	msg.addMethodArgument(account);
	submit(msg, "set_entry");
}

void SugarCrm::submit(QtSoapMessage msg, QString action)
{

	emit sendingMessage(msg.toXmlString());
	trans->setAction(action);
	trans->submitRequest(msg);

}

void SugarCrm::getUserInfo()
{
	QtSoapMessage msg = createMessage("get_user_id");
	msg.addMethodArgument("session", "", session);
	submit(msg, "get_user_id");
}

QtSoapMessage SugarCrm::createMessage(QString method)
{
	QtSoapMessage msg;
	msg.setMethod(method, "http://www.sugarcrm.com/sugarcrm");
	return msg;
}

void SugarCrm::processResponse(const QtSoapMessage msg)
{
	if(msg.isFault()) {
		qDebug() << msg.faultString().value().toString();
		emit returnedFaultyMessage(msg.faultString().value().toString());
		return;
	}

	QtSoapStruct &resp = (QtSoapStruct &) msg.method();
	Q_ASSERT(resp.count() != 0);


	QString action(resp.name().name());
	QtSoapStructIterator it(resp);

	if(it.data()->count() > 1) {

		decideAction(action, resp);
		return;
	}


	QMap <QString, QString> data;
	for( QtSoapStructIterator it(resp); it.current(); ++it)
	{
		//qDebug() << "item count" << it.data()->count();
		QtSoapType *item = it.data();
		//qDebug() << item->name().name() << ", " << item->value().toString();
		data.insert(item->name().name(), item->value().toString());
	}

	//qDebug() << resp.name().name();

	decideAction(action, data);
}

void SugarCrm::decideAction(const QString action, const QtSoapStruct data)
{
	/**
	 * login returns:
	 *
	 *	<message name="loginResponse">
	 *		<part name="return" type="tns:set_entry_result"/>
	 *	</message>
	 *
	 *	<xsd:complexType name="set_entry_result">
	 *		<xsd:all>
	 *			<xsd:element name="id" type="xsd:string"/>
	 *			<xsd:element name="error" type="tns:error_value"/>
	 *		</xsd:all>
	 *	</xsd:complexType>
	 */
	if (action == "loginResponse") {
		if(data["return"]["error"]["number"].value().toInt() == 0) {
			session = data["return"]["id"].value().toString();
			loggedin = true;
			//qDebug() << session;
			emit loginSuccessful();
		} else {
			loggedin = false;
			emit loginFailed();
		}
		return;
	}

	/**
	 * get_available_modules returns:
	 *
	 *	<message name="get_available_modulesResponse">
	 *		<part name="return" type="tns:module_list"/>
	 *	</message>
	 *
	 *	<xsd:complexType name="module_list">
	 *		<xsd:all>
	 *			<xsd:element name="modules" type="tns:select_fields"/>
	 *			<xsd:element name="error" type="tns:error_value"/>
	 *		</xsd:all>
	 *	</xsd:complexType>
	 *
	 *	<xsd:complexType name="select_fields">
	 *		<xsd:complexContent>
	 *			<xsd:restriction base="SOAP-ENC:Array">
	 *				<xsd:attribute ref="SOAP-ENC:arrayType" wsdl:arrayType="xsd:string[]"/>
	 *			</xsd:restriction>
	 *		</xsd:complexContent>
	 *	</xsd:complexType>
	 */
	if(action == "get_available_modulesResponse") {
		QtSoapArray modules((QtSoapArray &) data["return"]["modules"]);
		for(QtSoapArrayIterator it(modules); it.current(); ++it) {
			availableModules.append(it.data()->toString());
		}
		return;
	}

	/**
	 * get_entry_list returns:
	 *
	 *	<message name="get_entry_listResponse">
	 *		<part name="return" type="tns:get_entry_list_result"/>
	 *	</message>
	 *
	 *	<xsd:complexType name="get_entry_list_result">
	 *		<xsd:all>
	 *			<xsd:element name="result_count" type="xsd:int"/>
	 *			<xsd:element name="next_offset" type="xsd:int"/>
	 *			<xsd:element name="field_list" type="tns:field_list"/>
	 *			<xsd:element name="entry_list" type="tns:entry_list"/>
	 *			<xsd:element name="error" type="tns:error_value"/>
	 *		</xsd:all>
	 *	</xsd:complexType>
	 *
	 *	<xsd:complexType name="entry_list">
	 *		<xsd:complexContent>
	 *			<xsd:restriction base="SOAP-ENC:Array">
	 *				<xsd:attribute ref="SOAP-ENC:arrayType" wsdl:arrayType="tns:entry_value[]"/>
	 *			</xsd:restriction>
	 *		</xsd:complexContent>
	 *	</xsd:complexType>
	 *
	 *	<xsd:complexType name="entry_value">
	 *		<xsd:all>
	 *			<xsd:element name="id" type="xsd:string"/>
	 *			<xsd:element name="module_name" type="xsd:string"/>
	 *			<xsd:element name="name_value_list" type="tns:name_value_list"/>
	 *		</xsd:all>
	 *	</xsd:complexType>
	 *
	 *	<xsd:complexType name="name_value_list">
	 *		<xsd:complexContent>
	 *			<xsd:restriction base="SOAP-ENC:Array">
	 *				<xsd:attribute ref="SOAP-ENC:arrayType" wsdl:arrayType="tns:name_value[]"/>
	 *			</xsd:restriction>
	 *		</xsd:complexContent>
	 *	</xsd:complexType>
	 *
	 *	<xsd:complexType name="name_value">
	 *		<xsd:all>
	 *			<xsd:element name="name" type="xsd:string"/>
	 *			<xsd:element name="value" type="xsd:string"/>
	 *		</xsd:all>
	 *	</xsd:complexType>
	 */
	if(action == "get_entry_listResponse") {
		entries->clear();
		QString parentId;
		QtSoapArray ent((QtSoapArray &) data["return"]["entry_list"]);

		//qDebug() << "number of entries" << ent.count();
		
		for(int i = 0; i < ent.count(); i++) {

			QtSoapStruct tmp((QtSoapStruct &) ent.at(i));

			QtSoapArray values((QtSoapArray &) tmp["name_value_list"]);

			QMap<QString, QString> listEntries;
			for (int j = 0; j < values.count(); j++) {

				QtSoapStruct pair((QtSoapStruct &) values.at(j));


				listEntries[pair["name"].value().toString()] = pair["value"].value().toString();
			}
			entries->insert(QString(listEntries.value("id")), listEntries);
			parentId = (listEntries["parent_id"].isEmpty()) ? listEntries["account_id"] : listEntries["parent_id"];
		}
		emit dataAvailable(parentId);
		return;
	}

	/**
	 * get_entry_result returns:
	 *
	 *	<xsd:complexType name="get_entry_result">
	 *		<xsd:all>
	 *			<xsd:element name="field_list" type="tns:field_list"/>
	 *			<xsd:element name="entry_list" type="tns:entry_list"/>
	 *			<xsd:element name="error" type="tns:error_value"/>
	 *		</xsd:all>
	 *	</xsd:complexType>
	 *
	 *	<xsd:complexType name="entry_list">
	 *		<xsd:complexContent>
	 *			<xsd:restriction base="SOAP-ENC:Array">
	 *				<xsd:attribute ref="SOAP-ENC:arrayType" wsdl:arrayType="tns:entry_value[]"/>
	 *			</xsd:restriction>
	 *		</xsd:complexContent>
	 *	</xsd:complexType>
	 *
	 *	<xsd:complexType name="entry_value">
	 *		<xsd:all>
	 *			<xsd:element name="id" type="xsd:string"/>
	 *			<xsd:element name="module_name" type="xsd:string"/>
	 *			<xsd:element name="name_value_list" type="tns:name_value_list"/>
	 *		</xsd:all>
	 *	</xsd:complexType>
	 *
	 *	<xsd:complexType name="name_value_list">
	 *		<xsd:complexContent>
	 *			<xsd:restriction base="SOAP-ENC:Array">
	 *				<xsd:attribute ref="SOAP-ENC:arrayType" wsdl:arrayType="tns:name_value[]"/>
	 *			</xsd:restriction>
	 *		</xsd:complexContent>
	 *	</xsd:complexType>
	 *
	 *	<xsd:complexType name="name_value">
	 *		<xsd:all>
	 *			<xsd:element name="name" type="xsd:string"/>
	 *			<xsd:element name="value" type="xsd:string"/>
	 *		</xsd:all>
	 *	</xsd:complexType>
	 */
	if(action == "get_related_notesResponse") {
		notes.clear();
		QString parentId;
		QtSoapArray ent((QtSoapArray &) data["return"]["entry_list"]);
		for(int i = 0; i < ent.count(); i++) {

			QtSoapStruct tmp((QtSoapStruct &) ent.at(i));

			QtSoapArray values((QtSoapArray &) tmp["name_value_list"]);

			QMap<QString, QString> listEntries;
			for (int j = 0; j < values.count(); j++) {

				QtSoapStruct pair((QtSoapStruct &) values.at(j));


				listEntries[pair["name"].value().toString()] = pair["value"].value().toString();
			}
			notes.insert(QString(listEntries.value("id")), listEntries);
			parentId = (listEntries["contact_id"].isEmpty()) ? listEntries["parent_id"] : listEntries["contact_id"];
		}
		emit notesAvailable(parentId);
		return;
	}

	/**
	 * set_entry returns:
	 *
	 *	<message name="set_entryResponse">
	 *		<part name="return" type="tns:set_entry_result"/>
	 *	</message>
	 *
	 *	<xsd:complexType name="set_entry_result">
	 *		<xsd:all>
	 *			<xsd:element name="id" type="xsd:string"/>
	 *			<xsd:element name="error" type="tns:error_value"/>
	 *		</xsd:all>
	 *	</xsd:complexType>
	 */
	if(action == "set_entryResponse" ||
	   action == "set_note_attachmentResponse")
	{
		//qDebug() << data["return"]["id"].value().toString();
		emit entryCreated(data["return"]["id"].value().toString());
		emit entryUpdated(data["return"]["id"].value().toString());
		return;
	}

	/**
	 * get_note_attachment returns
	 *
	 *	<message name="get_note_attachmentResponse">
	 *		<part name="return" type="tns:return_note_attachment"/>
	 *	</message>
	 *
	 *	<xsd:complexType name="return_note_attachment">
	 *		<xsd:all>
	 *			<xsd:element name="note_attachment" type="tns:note_attachment"/>
	 *			<xsd:element name="error" type="tns:error_value"/>
	 *		</xsd:all>
	 *	</xsd:complexType>
	 *
	 *	<xsd:complexType name="note_attachment">
	 *		<xsd:all>
	 *			<xsd:element name="id" type="xsd:string"/>
	 *			<xsd:element name="filename" type="xsd:string"/>
	 *			<xsd:element name="file" type="xsd:string"/>
	 *		</xsd:all>
	 *	</xsd:complexType>
	 */
	if(action == "get_note_attachmentResponse") {
		//qDebug() << data["return"]["note_attachment"]["id"].toString();
		noteAttachment = data["return"]["note_attachment"]["file"].toString();
		emit attachmentAvailable(data["return"]["note_attachment"]["id"].toString());
		return;
	}

	emit unknownAction(action);
}

void SugarCrm::decideAction(const QString action, const QMap<QString, QString> data)
{
	/**
	 * create_session returns:
	 *
	 *	<message name="create_sessionResponse">
	 *		<part name="return" type="xsd:string"/>
	 *	</message>
	 */
	if (action == "create_sessionResponse") {
		if(data.value("return").contains("Success")) {
			loggedin = true;
			emit loginSuccessful();
		} else {
			loggedin = false;
			emit loginFailed();
		}
		return;
	}

	/**
	 * get_server_time returns:
	 *
	 *	<message name="get_server_timeResponse">
	 *		<part name="return" type="xsd:string"/>
	 *	</message>
	 */
	if (action == "get_server_timeResponse") {
		serverTime = QDateTime::fromString(data.value("return"), "YYYY-MM-dd HH:mm:ss");
		return;
	}

	/**
	 * get_user_id returns:
	 *
	 *	<message name="get_user_idResponse">
	 *		<part name="return" type="xsd:string"/>
	 *	</message>
	 */
	if (action == "get_user_idResponse") {
		uid = data.value("return");
		return;
	}

	emit unknownAction(action);
}
