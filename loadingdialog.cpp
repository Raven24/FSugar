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

#include <QtGui>
#include "loadingdialog.h"

LoadingDialog::LoadingDialog(QWidget *parent) :
    QDialog(parent)
{
	setWindowFlags(Qt::FramelessWindowHint);

	message = new QLabel(tr("laden..."), this);
	label = new QLabel(this);

	QMovie *mov = new QMovie(this);
	mov->setCacheMode(QMovie::CacheAll);
	label->setMovie(mov);
	mov->setFileName(":loading.gif");
	mov->start();
}

void LoadingDialog::setText(QString msg)
{
	message->setText(msg);
}

void LoadingDialog::paintEvent(QPaintEvent *)
{
	QRect r(this->parentWidget()->rect());
	resize(this->parentWidget()->size());
#ifndef Q_WS_X11
	move(0, 0);
#endif
	QPainter painter(this);
	painter.setPen(Qt::NoPen);
	painter.setBrush(QColor(65, 65, 65, 100));
	painter.drawRect(r);
	r.setSize(QSize(160, 160));
	r.moveTopLeft(QPoint(this->rect().width()/2 - 80,
						 this->rect().height()/2 - 80));
	painter.drawRoundedRect(r, 5, 5, Qt::AbsoluteSize);

	drawContents();
}

void LoadingDialog::drawContents()
{

	QRect r(QPoint(this->rect().width()/2,
					this->rect().height()/2),
			 label->size());
	r.moveRight(r.x() + r.width()/2);
	r.moveTop(r.y() - 30 );

	label->setGeometry(r);

	r.setSize(message->size());
	r.moveBottom(r.y()+r.height()+40);

	message->setGeometry(r);

}
