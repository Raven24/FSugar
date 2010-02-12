#ifndef DOCKWIDGET_H
#define DOCKWIDGET_H

#include <QDockWidget>
#include <QTextBrowser>

class TextBrowser : public QTextBrowser
{
	Q_OBJECT
public:
	TextBrowser(QWidget *parent = 0);
	QSize sizeHint();
	QSize minimumSize();
	void append(const QString &text);
};

class DockWidget : public QDockWidget
{
Q_OBJECT
public:
	explicit DockWidget(QWidget *parent = 0);
	TextBrowser *text;
	QSize sizeHint();
	QSize maximumSize();

signals:

public slots:

};



#endif // DOCKWIDGET_H
