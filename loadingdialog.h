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
