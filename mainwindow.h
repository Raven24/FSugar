#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWebKit>

#include "logindialog.h"
#include "loadingdialog.h"
#include "dockwidget.h"
#include "sugarcrmsoap.h"
#include "sugarcrm.h"
#include "sugarsettings.h"
#include "accountmodel.h"
#include "accountproxymodel.h"
#include "accountdetail.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

	static QString appPath;
	static MainWindow* getInstance();

	QTabWidget *mainWidget;

signals:
	void listReady();

public slots:
	void unknownAction(QString action);
	void setStatusMsg(QString msg, int time=1500);

	void displayAccounts();
	void displayAccount(const QModelIndex index);
	void displayPressAccount(const QModelIndex index);
	void addAccount();
	void displayCalendar();
	void displayPressList();
	void displaySettings();

protected:
    void changeEvent(QEvent *e);
	void paintEvent(QPaintEvent * event);

private:
    Ui::MainWindow *ui;

	static MainWindow *instance;

	LoginDialog *loginDialog;
	LoadingDialog *loadingDialog;
	DockWidget *dockWidget;
	QToolBar *toolBar;
	SugarCrm *crm;
	SugarSettings *settings;

	QAction *addAccountAct, *openCalAct, *pressViewAct;
	QListView *listView, *pressList;
	AccountDetail *accountDetail;
	QWebView *webView;

	AccountModel *accountModel;
	AccountProxyModel *filterModel;

private slots:
	void login();
	void doSearch();
	void loginResponse();
	void debug(QString msg);

	void closeAccountTab(const int index);

};

#endif // MAINWINDOW_H
