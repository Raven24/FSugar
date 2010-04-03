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

#include <QtCore>
#include <QtGui>
#include "dockwidget.h"

DockWidget::DockWidget(QWidget *parent) :
    QDockWidget(parent)
{
	setWindowTitle(tr("Server-Kommunikation"));
	setFixedHeight(100);

	text = new TextBrowser;
	setWidget(text);
}

QSize DockWidget::maximumSize()
{
	return widget()->sizeHint();
}

QSize DockWidget::sizeHint()
{
	return minimumSize();
}

TextBrowser::TextBrowser(QWidget *parent) :
		QTextBrowser(parent)
{
	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
	setFontPointSize(8);
	setFontFamily("Monospace");
}

QSize TextBrowser::sizeHint()
{
	QSize s = QTextBrowser::sizeHint();
	s.setHeight(60);
	qDebug() << "size hint " << s;
	return s;
}

QSize TextBrowser::minimumSize()
{
	return sizeHint();
}

void TextBrowser::append(const QString &text)
{
	setText(toPlainText().append("\n").append(text));
	verticalScrollBar()->setValue(verticalScrollBar()->maximum());
}
