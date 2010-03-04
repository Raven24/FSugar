#include <QtGui>
#include <QtWebKit>

#include "mainwindow.h"
#include "logindialog.h"
#include "dockwidget.h"
#include "ui_mainwindow.h"

#include "sugarcrmsoap.h"
#include "sugarcrm.h"
#include "qtsoap.h"
#include "accountmodel.h"
#include "accountproxymodel.h"
#include "accountdetail.h"
#include "settingsdialog.h"
#include "cookiejar.h"

QString MainWindow::appPath = QString("");
MainWindow* MainWindow::instance = NULL;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	setWindowIcon(QIcon(":icon.ico"));

	MainWindow::appPath = qApp->applicationDirPath();

	dockWidget = new DockWidget;
	addDockWidget(Qt::BottomDockWidgetArea, dockWidget);
	dockWidget->hide();

	toolBar = new QToolBar(tr("Aktionen"));
	toolBar->setIconSize(QSize(42, 42));
	toolBar->setMovable(false);

	addAccountAct = new QAction(QIcon(":add-account.png"), tr("Neue Firma"), this);
	openCalAct = new QAction(QIcon(":calendar.png"), tr("Kalender"), this);
	pressViewAct = new QAction(QIcon(":news.png"), tr("Pressekontakte"), this);
	//toolBar->addAction(QIcon(":add-contact.png"), tr("Neuer Kontakt"));
	//toolBar->addAction(QIcon(":add-task.png"),    tr("Neue Aufgabe"));

	connect(addAccountAct, SIGNAL(triggered()),
			this, SLOT(addAccount()));
	connect(openCalAct, SIGNAL(triggered()),
			this, SLOT(displayCalendar()));
	connect(pressViewAct, SIGNAL(triggered()),
			this, SLOT(displayPressList()));

	toolBar->addAction(addAccountAct);
	toolBar->addAction(openCalAct);
	toolBar->addAction(pressViewAct);

	addToolBar(Qt::LeftToolBarArea, toolBar);
	toolBar->hide();

	QPushButton *loginBtn = new QPushButton(QIcon(":login.png"), tr("Login"), this);
	connect(loginBtn, SIGNAL(pressed()),
			this, SLOT(login()));
	QGridLayout *l = new QGridLayout(this);
	QWidget *c = new QWidget(this);
	l->addWidget(loginBtn, 1, 1, Qt::AlignCenter);
	c->setLayout(l);
	setCentralWidget(c);

	// initialize dialogs
	loadingDialog = new LoadingDialog(this);
	loginDialog   = new LoginDialog(this);
	loadingDialog->setVisible(false);
	loginDialog->setVisible(false);

	crm = SugarCrm::getInstance();
	accountModel = AccountModel::getInstance();
	filterModel = new AccountProxyModel(this);
	filterModel->setSourceModel(accountModel);
	settings = SugarSettings::getInstance();;

        // QML display
	//centerView = new QmlView(this);

	//centerView->setUrl(QUrl("SugarCrm.qml"));
	//centerView->setAttribute(Qt::WA_OpaquePaintEvent);
	//centerView->setAttribute(Qt::WA_NoSystemBackground);
	//centerView->setFocus();

	//contx = centerView->rootContext();
	//contx->addDefaultObject(this);
	//contx->setContextProperty("qmlViewer", this);
	//contx->setContextProperty("accountModel", proxyModel);

	connect(ui->actionLogin, SIGNAL(triggered()),
			this, SLOT(login()));
	connect(ui->actionEinstellungen, SIGNAL(triggered()),
			this, SLOT(displaySettings()));
	connect(ui->actionLogout, SIGNAL(triggered()),
			qApp, SLOT(quit()));
	connect(ui->actionServer_Zeit, SIGNAL(triggered()),
			crm, SLOT(getServerTime()));
	connect(ui->actionSugarFlavor, SIGNAL(triggered()),
			crm, SLOT(getSugarFlavor()));
	connect(ui->actionAboutQt, SIGNAL(triggered()),
			qApp, SLOT(aboutQt()));

	// DEBUG XML COMMUNICATION
	connect(crm, SIGNAL(sendingMessage(QString)),
			this, SLOT(debug(QString)));
	connect(crm->trans, SIGNAL(newSoapMessage(QString)),
			this, SLOT(debug(QString)));

	connect(crm, SIGNAL(unknownAction(QString)),
			this, SLOT(unknownAction(QString)));

	// login response
	connect(crm, SIGNAL(loginFailed()),
			this, SLOT(loginResponse()));
	connect(crm, SIGNAL(loginSuccessful()),
			this, SLOT(loginResponse()));


	//TODO: change this when it works
	//setCentralWidget(centerView);
	//centerView->execute();

	//rootComponent = centerView->root();
	//QVariant *tmp = new QVariant(contx->property("authView"));
	//QmlComponent authComponent((QObject *) tmp);  // centerView->engine(), QUrl("content/AuthView.qml"));
	//authView = authComponent.create(contx);
	//qDebug() << authView->dynamicPropertyN();

	//connect(rootComponent, SIGNAL(loggingIn()),
	//		this, SLOT(login()));
	//connect(rootComponent, SIGNAL(searching()),
	//		this, SLOT(doSearch()));
	/*connect(accountModel, SIGNAL(dataReady()),
			this, SLOT(assignData()));
	connect(this, SIGNAL(listReady()),
			rootComponent, SLOT(displayAccounts()));*/
	//connect(accountModel, SIGNAL(dataReady()),
	//		rootComponent, SLOT(displayAccounts()));


	// ordinary display
	listView = new QListView();
	listView->setAlternatingRowColors(true);
	pressList = new QListView();
	pressList->setAlternatingRowColors(true);

	QNetworkAccessManager *nwManager = new QNetworkAccessManager();
	nwManager->setCookieJar(new CookieJar(this));
	webView = new QWebView();
	webView->page()->setNetworkAccessManager(nwManager);

	mainWidget = new QTabWidget(this);
	mainWidget->setVisible(false);
	mainWidget->setTabsClosable(true);
	mainWidget->addTab(listView, tr("Liste"));

	connect(accountModel, SIGNAL(dataReady()),
			this, SLOT(displayAccounts()));
	connect(listView, SIGNAL(doubleClicked(QModelIndex)),
			this, SLOT(displayAccount(QModelIndex)));
	connect(pressList, SIGNAL(doubleClicked(QModelIndex)),
			this, SLOT(displayPressAccount(QModelIndex)));
	connect(mainWidget, SIGNAL(tabCloseRequested(int)),
			this, SLOT(closeAccountTab(int)));

	setStatusMsg(tr("Bereit"), 2500);
}

void MainWindow::displayAccounts()
{
	loadingDialog->hide();
	listView->setModel(accountModel);
	setCentralWidget(mainWidget);
	toolBar->show();
	mainWidget->show();
}

void MainWindow::displayAccount(const QModelIndex index)
{
	//qDebug() << index;	
	mainWidget->setCurrentIndex(mainWidget->addTab(new AccountDetail(&index), tr("Detailansicht")));
}

void MainWindow::displayPressAccount(const QModelIndex index)
{
	displayAccount(filterModel->mapToSource(index));
}

void MainWindow::closeAccountTab(const int index)
{
	QWidget *tab = mainWidget->widget(index);
	mainWidget->removeTab(index);
	if(!tab->property("doNotDelete").toBool()) {
		tab->deleteLater();
	}
}

MainWindow::~MainWindow()
{
	delete webView->page()->networkAccessManager()->cookieJar();
	delete ui;
}

MainWindow* MainWindow::getInstance()
{
	if(MainWindow::instance == NULL) {
		MainWindow::instance = new MainWindow();
	}
	return MainWindow::instance;
}

void MainWindow::addAccount()
{
	//qDebug() << "here we will show the form to add an account";
	mainWidget->setCurrentIndex(mainWidget->addTab(new AccountDetail(accountModel->newAccount()), tr("Neue Firma")));
}

void MainWindow::displaySettings()
{
	if(mainWidget->isHidden()) {
		setCentralWidget(new SettingsDialog);
		return;
	}

	mainWidget->setCurrentIndex(mainWidget->addTab(new SettingsDialog, tr("Einstellungen")));
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
	QWidget::paintEvent(event);

	//contx->setContextProperty("windowWidth", centralWidget()->geometry().width());
	//contx->setContextProperty("windowHeight", centralWidget()->geometry().height());
}

void MainWindow::login()
{
	QString user;
	QByteArray pass;
	centralWidget()->hide();

	// ordinary table view
	if(loginDialog->exec() == QDialog::Accepted) {
		loadingDialog->show();
		user = loginDialog->getUsername();
		pass = loginDialog->getPassword().toUtf8();
	} else {
		return;
	}

	// qml login
	//user = rootComponent->property("username").toString();
	//pass = rootComponent->property("password").toString().toUtf8();

	QString md5pass( QCryptographicHash::hash(pass, QCryptographicHash::Md5).toHex() );

	crm->login(user, md5pass);
}

void MainWindow::loginResponse()
{
	if(crm->loggedin) {
		setStatusMsg(tr("Login erfolgreich!"), 2000);
		accountModel->fetchData();
		return;
	}

	loadingDialog->hide();
	//rootComponent->setProperty("state", "loginFailed");
	setStatusMsg(tr("Login fehlgeschlagen!"));
}

void MainWindow::displayCalendar()
{
	webView->setProperty("doNotDelete", true);
	webView->load(QUrl(settings->calendarUrl));
	mainWidget->setCurrentIndex(mainWidget->addTab(webView, tr("Kalender")));
}

void MainWindow::debug(QString msg)
{
	Q_UNUSED(msg)

	setStatusMsg(tr("Antwort erhalten"), 1000);
	//dockWidget->text->append(msg);
	qDebug() << msg;

}

void MainWindow::unknownAction(QString action)
{
	setStatusMsg(tr("Unbekannte Antwort: '%1'").arg(action), 2500);
}

void MainWindow::displayPressList()
{
	//qDebug() << "here we show the list through a filter";
	filterModel->setFilterRole(256); // 265 == acc.category;
	filterModel->setFilterRegExp(QRegExp("press", Qt::CaseInsensitive, QRegExp::FixedString));

	pressList->setModel(filterModel);
        pressList->setProperty("doNotDelete", true);

	mainWidget->setCurrentIndex(mainWidget->addTab(pressList, tr("Pressekontakte")));
}

void MainWindow::doSearch()
{

}

void MainWindow::setStatusMsg(QString msg, int time)
{
	statusBar()->showMessage(msg, time);
}
