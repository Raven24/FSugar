#include <QtCore>
#include <QtGui>

#include "abstractitemdetail.h"

AbstractItemDetail::AbstractItemDetail(QWidget *parent) :
    QWidget(parent)
{
}

AbstractItemDetail::AbstractItemDetail(const QModelIndex *index)
{
	Q_UNUSED(index)

	initDialog();
	retrieveItem();
	fillData();
}

void AbstractItemDetail::paintEvent(QPaintEvent *)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void AbstractItemDetail::initDialog()
{
	newNoteDialog = new CreateNoteDialog(this);
	newNoteDialog->hide();

	notesModel = new NotesModel(this);

	loading = new QLabel(this);
	QMovie *mov = new QMovie();
	mov->setCacheMode(QMovie::CacheAll);
	loading->setMovie(mov);
	mov->setFileName(":loading.gif");
	mov->setScaledSize(QSize(20, 20));
	mov->start();

	crm = SugarCrm::getInstance();

	notesTable = new QTableView(this);
	notesTable->verticalHeader()->hide();
	notesTable->horizontalHeader()->setStretchLastSection(true);
	notesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	notesTable->setSelectionMode(QAbstractItemView::SingleSelection);

	// buttons
	save = new QPushButton(QIcon(":save.png"), tr("Speichern"));
	newNote = new QPushButton(QIcon(":notes.png"), tr("Neue Notiz"));
	newDocument = new QPushButton(QIcon(":documents.png"), tr("Neues Dokument"));

	connect(save, SIGNAL(pressed()),
			this, SLOT(saveChanges()));
	connect(newNote, SIGNAL(pressed()),
			this, SLOT(showNewNoteDialog()));
	connect(newNoteDialog, SIGNAL(accepted()),
			this, SLOT(createNewNote()));
	connect(newDocument, SIGNAL(pressed()),
			this, SLOT(showNewDocumentDialog()));
	connect(notesTable, SIGNAL(doubleClicked(QModelIndex)),
			this, SLOT(downloadNoteAttachment(QModelIndex)));
}

void AbstractItemDetail::afterSaveAct()
{
	progress(false);
	hideButtons(false);

	getItem()->getChildren();
}

void AbstractItemDetail::displayNotes()
{
	notesModel->read(&(getItem()->notes));
	notesTable->setModel(notesModel);
	notesTable->resizeRowsToContents();
	notesTable->resizeColumnToContents(1);
	progress(false);
}

void AbstractItemDetail::createNewNote()
{
	progress(true);

	Note *newNote = notesModel->newNote();
	newNote->name = newNoteDialog->noteName;
	newNote->description = newNoteDialog->noteDescription;
	if(!newNoteDialog->fileName.isEmpty())
		newNote->fileName = newNoteDialog->fileName;
	newNote->parentType = getItem()->type;
	newNote->parentId = getItem()->id;

	newNote->save();

	connect(newNote, SIGNAL(saved()),
			this, SLOT(displayNotes()));
}

void AbstractItemDetail::progress(bool p)
{
	if(p) loading->show();
	else loading->hide();
}

void AbstractItemDetail::hideButtons(const bool _var)
{
	if(_var) {
		newNote->hide();
		newDocument->hide();
		notesTable->hide();
	} else {
		newNote->show();
		newDocument->show();
		notesTable->show();
	}
}

void AbstractItemDetail::showNewDocumentDialog()
{
	newNoteDialog->setUpload(true);
	newNoteDialog->show();
}

void AbstractItemDetail::showNewNoteDialog()
{
	newNoteDialog->setUpload(false);
	newNoteDialog->show();
}

void AbstractItemDetail::downloadNoteAttachment(const QModelIndex _index)
{
	if (getItem()->notes.at(_index.row())->fileName.isEmpty()) return;

	progress(true);
	connect(getItem()->notes.at(_index.row()), SIGNAL(openingAttachment()),
			this, SLOT(endProgress()));
	getItem()->notes.at(_index.row())->downloadAttachment();
}

void AbstractItemDetail::endProgress()
{
	progress(false);
}

void AbstractItemDetail::fillData()
{
}

void AbstractItemDetail::retrieveItem()
{
}
