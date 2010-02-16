#include "qtsoap.h"
#include "sugarcrmsoap.h"
#include "sugarcrm.h"
#include "sugarsettings.h"

SugarCrm *SugarCrm::instance = NULL;

SugarCrm::SugarCrm(QObject *parent) :
    QObject(parent)
{
	settings = new SugarSettings();
	trans = new SugarCrmSoap(settings->sugarHost);

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

void SugarCrm::createNote(const QString _module, const QString _name, const QString _description, const QString _parentType, const QString _parentId)
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

	QtSoapArray *note = new QtSoapArray(QtSoapQName("name_value_list"));
	note->insert(0, name);
	note->insert(1, desc);
	note->insert(2, parType);
	note->insert(3, parId);

	QtSoapMessage msg = createMessage("set_entry");
	msg.addMethodArgument("session", "", session);
	msg.addMethodArgument("module_name", "", _module);
	msg.addMethodArgument(note);
	submit(msg, "set_entry");
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

void SugarCrm::getEntryList(const QString _module, const QString _query,
							const QString _orderBy, const int _offset,
							const int _maxResults, const int _deleted)
{
	QtSoapMessage msg = createMessage("get_entry_list");
	msg.addMethodArgument("session", "", session);
	msg.addMethodArgument("module_name", "", _module );
	msg.addMethodArgument("query", "", _query);
	msg.addMethodArgument("order_by", "", _orderBy);
	msg.addMethodArgument("offset", "", _offset);
	msg.addMethodArgument("select_fields", "", "");
	msg.addMethodArgument("max_results", "", _maxResults);
	msg.addMethodArgument("deleted", "", _deleted);

	submit(msg, "get_entry_list");
}

void SugarCrm::getRelatedNotes(const QString _module, const QString _id)
{
	notes.clear();

	QtSoapMessage msg = createMessage("get_related_notes");
	msg.addMethodArgument("session", "", session);
	msg.addMethodArgument("module_name", "", _module);
	msg.addMethodArgument("module_id", "", _id);
	submit(msg, "get_related_notes");
}

void SugarCrm::updateAccount(const QString _id, const QString _name, const QString _description,
							 const QString _addressStreet, const QString _addressCity, const QString _addressPostalcode,
							 const QString _addressCountry, const QString _phoneOffice, const QString _phoneFax,
							 const QString _phoneAlternate, const QString _email, const QString _website)
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
	trans->submitRequest(msg, settings->sugarPath);

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
		entries.clear();
		QtSoapArray ent((QtSoapArray &) data["return"]["entry_list"]);
		for(int i = 0; i < ent.count(); i++) {

			QtSoapStruct tmp((QtSoapStruct &) ent.at(i));

			QtSoapArray values((QtSoapArray &) tmp["name_value_list"]);

			QMap<QString, QString> listEntries;
			for (int j = 0; j < values.count(); j++) {

				QtSoapStruct pair((QtSoapStruct &) values.at(j));


				listEntries[pair["name"].value().toString()] = pair["value"].value().toString();
			}
			entries.insert(QString(listEntries.value("id")), listEntries);

		}
		//qDebug() << entries.size() << "=" << ent.count();
		emit dataAvailable();
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
			parentId = listEntries["parent_id"];
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
	if(action == "set_entryResponse") {
		//qDebug() << data["return"]["id"].value().toString();
		emit entryCreated(data["return"]["id"].value().toString());
		emit entryUpdated(data["return"]["id"].value().toString());
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
