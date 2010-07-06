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

#ifndef SEARCHFIELD_H
#define SEARCHFIELD_H

#include <QtGui>

class SearchField : public QWidget
{
Q_OBJECT
public:
    explicit SearchField(QWidget *parent = 0);

signals:
	void searchPhraseChanged(QString phrase);

public slots:

protected:
	QPushButton *searchBtn;
	QLineEdit *searchField;

};

#endif // SEARCHFIELD_H
