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
	r.moveTopLeft(QPoint(r.width()/2, r.height()/2));
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
