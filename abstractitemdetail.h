#ifndef ABSTRACTITEMDETAIL_H
#define ABSTRACTITEMDETAIL_H

#include <QWidget>

class AbstractItemDetail : public QWidget
{
Q_OBJECT
public:
	AbstractItemDetail(QWidget *parent = 0);

signals:

public slots:
	void displayNotes();
	void saveChanges();
	void progress(bool _p = false);
	void endProgress();
	void showNewDocumentDialog();
	void showNewNoteDialog();
	void downloadAttachmentNote();

	virtual void initDialog();

protected:
	void paintEvent(QPaintEvent *);

};

#endif // ABSTRACTITEMDETAIL_H
