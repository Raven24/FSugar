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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "logindialog.h"
#include "loadingdialog.h"
#include "dockwidget.h"
#include "sugarcrmsoap.h"
#include "sugarcrm.h"
#include "sugarsettings.h"
#include "accountmodel.h"
#include "accountproxymodel.h"
#include "accountdetail.h"
#include "searchfield.h"
#include "accountlist.h"
#include "calendarwidget.h"
#include "settingsdialog.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

	static QString appPath;
	static QUrl projectWebsite;
	static MainWindow* getInstance();

	QTabWidget *mainWidget;

signals:
	void listReady();

public slots:
	void unknownAction(QString action);
	void setStatusMsg(QString msg, int time=1500);

	void displayAccounts();
// 	void displayAccount(const QModelIndex index);
// 	void displayPressAccount(const QModelIndex index);
// 	void displayFilteredAccount(const QModelIndex index);
	void addAccount();
	void displayCalendar();
	void displayPressList();
	void displaySettings();
	void openProjectHomepage();

	void cleanup();

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
	QStackedLayout *mainLayout;
	QListView *listView, *pressList;
	AccountDetail *accountDetail;

	AccountModel *accountModel;
	AccountProxyModel *filterModel, *searchFilterModel;
	AccountList *accountList;
	CalendarWidget *calWidget;
	SettingsDialog *settingsDialog;

private slots:
	void login();
// 	void doSearch(QString phrase);
	void loginResponse();
	void debug(QString msg);

// 	void closeAccountTab(const int index);
	void loadStyle();

};

#endif // MAINWINDOW_H
