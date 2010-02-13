#ifndef CREATENOTEDIALOG_H
#define CREATENOTEDIALOG_H

#include <QDialog>

class QLineEdit;
class QTextEdit;
class QPushButton;
class QString;

class CreateNoteDialog : public QDialog
{
	Q_OBJECT

public:
	CreateNoteDialog(QWidget *parent = 0);

	QString noteName, noteDescription;

public slots:
	void saveNote();

private:
	QLineEdit *name;
	QTextEdit *description;

};

#endif // CREATENOTEDIALOG_H
