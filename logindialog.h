#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

class QLineEdit;
class QPushButton;
class QString;

class LoginDialog : public QDialog
{
	Q_OBJECT

public:
	LoginDialog(QWidget *parent = 0);
	~LoginDialog();
	QString getUsername();
	QString getPassword();

private:
	QLineEdit *user, *pass;
	QPushButton *ok, *abort;
};

#endif // LOGINDIALOG_H
