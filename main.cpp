#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

	qApp->setStyleSheet("QWidget { background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop:0 #333, stop:0.3 #232323 stop: 1 #333); color: #FFF; }"
						"AccountDetail { background: qradialgradient(cx:0, cy:-0.2, radius: 1, fx:0.5, fy:0.5, stop:0 #888, stop:1 #555); } "
						"LoadingDialog { background: none; }"

						"QMenuBar { background: #222; border-bottom: 1px solid #BBB; padding: 2px; }"
						"QMenuBar::item { padding: 4px 8px; spacing: 2px; border-radius: 4px; }"
						"QMenuBar::item:selected { border: none; background: qradialgradient(cx:0.5, cy:0, radius: 1, fx:0.5, fy:0, stop:0 #777, stop:1 #333); }"

						"QMenu { border: 1px solid #111; }"
						"QMenu::item { padding: 4px 4px 4px 22px; margin-top: 2px; }"
						"QMenu::item:selected { background: #777; border-radius: 4px; }"
						"QMenu::item::icon:selected { border: none; }"

						"QListView { background: url(:stripes.png); alternate-background-color: rgba(0, 0, 0, 60%); border: 1px solid #333; }"
						"QListView::item { padding: 4px 2px; }"

						"QTabWidget::pane { border: 1px solid #222; margin-top: 7px; }"
						"QTabBar::tab { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop: 0 #444, stop: 1 #888); border-top-left-radius: 4px; border-top-right-radius: 4px; margin-left: 2px; padding: 4px 8px; }"

						"QTableView QHeaderView::section { background: #000; border: 1px solid #000; border-right: 1px solid #454545; color: #AAA; }"
						"QTableView { border: 1px solid #222; }"

						"QLabel { color: rgba(255, 255, 255, 70%); font-size: 9px; background: none; } "
						"QLineEdit, QTextEdit { background: #FdFdFd; border: 1px solid qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop:0 white, stop: 1 #989889); color: #010101; } "
						"QPushButton { border: none; background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #343434, stop: 0.7 #222, stop: 1 #121212); color: #FFF; border-radius: 4px; padding: 5px; }"
						"QPushButton:pressed { background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #121212, stop: 0.7 #222, stop: 1 #343434); }"
					   );

    return a.exec();
}

