#include <QtCore>
#include <QtGui>

#include "abstractitemdetail.h"

AbstractItemDetail::AbstractItemDetail(QWidget *parent) :
    QWidget(parent)
{
}

void AbstractItemDetail::paintEvent(QPaintEvent *)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
