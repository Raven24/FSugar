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

#ifndef LOADINGDIALOG_H
#define LOADINGDIALOG_H

#include <QDialog>

class QLabel;

class LoadingDialog : public QDialog
{
Q_OBJECT

public:
	LoadingDialog(QWidget *parent = 0);
	void setText(QString msg);
	void paintEvent(QPaintEvent *);

	QLabel *message;
	QLabel *label;

signals:

public slots:

private:
	void drawContents();

};

#endif // LOADINGDIALOG_H
