#ifndef CREATENOTEDIALOG_H
#define CREATENOTEDIALOG_H

#include <QDialog>

class QLineEdit;
class QTextEdit;
class QPushButton;
class QString;
class QWidget;

class CreateNoteDialog : public QDialog
{
	Q_OBJECT

public:
	CreateNoteDialog(QWidget *parent = 0);
	void setUpload(const bool val);

	QString noteName, noteDescription, fileName;

public slots:
	void saveNote();
	void showFileChooser();

private:
	QLineEdit *name, *fileNameEdit;
	QTextEdit *description;
	QWidget *fileLayout;
	bool fileUpload;

};

#endif // CREATENOTEDIALOG_H
