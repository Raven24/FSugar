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

#include "searchfield.h"

SearchField::SearchField(QWidget *parent) :
    QWidget(parent)
{

	searchBtn = new QPushButton(QIcon(":search.png"), "");
	searchBtn->setProperty("btnType", "right");
	searchBtn->setToolTip(tr("Suchen"));
	searchBtn->setFlat(true);

	searchField = new QLineEdit();
	searchField->setPlaceholderText(tr("Suche"));
	searchField->installEventFilter(this);

	QHBoxLayout *layout = new QHBoxLayout();
	layout->setSpacing(0);
	layout->setContentsMargins(QMargins(0, 1, 3, 0));
	layout->addWidget(searchField, 3);
	layout->addWidget(searchBtn, 0 , Qt::AlignLeft);

	connect(searchField, SIGNAL(textChanged(QString)),
			this, SIGNAL(searchPhraseChanged(QString)));

	setLayout(layout);
}

bool SearchField::eventFilter(QObject *obj, QEvent *event)
{
	if(obj != searchField) {
		return QWidget::eventFilter(obj, event);
	}

	if(event->type() == QEvent::KeyPress) {
		int key = static_cast<QKeyEvent*>(event)->key();
		if(key == Qt::Key_Escape) {
			searchField->clear();
			return true;
		}
	}
	return false;
}
