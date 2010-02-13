#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "logindialog.h"
#include "loadingdialog.h"
#include "dockwidget.h"
#include "sugarcrmsoap.h"
#include "sugarcrm.h"
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

signals:
	void listReady();

public slots:
	void unknownAction(QString action);
	void setStatusMsg(QString msg, int time=1500);

	void displayAccounts();
	void displayAccount(const QModelIndex index);

protected:
    void changeEvent(QEvent *e);
	void paintEvent(QPaintEvent * event);

private:
    Ui::MainWindow *ui;

	LoginDialog *loginDialog;
	LoadingDialog *loadingDialog;
	DockWidget *dockWidget;
	SugarCrm *crm;

	// ordinary view
	QListView *listView;
	QTabWidget *mainWidget;
	AccountDetail *accountDetail;

	AccountModel *accountModel;
	AccountProxyModel *proxyModel;

private slots:
	void login();
	void doSearch();
	void loginResponse();
	void debug(QString msg);

	void closeAccountTab(const int index);

};

#endif // MAINWINDOW_H
