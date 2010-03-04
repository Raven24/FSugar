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
