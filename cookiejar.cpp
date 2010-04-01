#include <QtCore>
#include "cookiejar.h"
#include "mainwindow.h"

CookieJar::CookieJar(QObject* parent) :
		QNetworkCookieJar(parent)
{
	populateCookies();
	//qDebug() << "number of cookies read:" << allCookies().size();
}

CookieJar::~CookieJar()
{
	saveCookies();

}

void CookieJar::populateCookies()
{
	QList<QNetworkCookie> list;

	QDir::setCurrent(MainWindow::appPath); // make sure we are in the app's dir
	QFile f("cookies.txt");
	if(!f.open(QIODevice::ReadOnly)) {
		qDebug() << "could not open cookies";
		return;
	}
	while(!f.atEnd()) {
		QList<QNetworkCookie> c = QNetworkCookie::parseCookies(f.readLine());
		list.append(c);
	}

	setAllCookies(list);
}

void CookieJar::saveCookies()
{
	//qDebug() << "saving cookies";

	QDir::setCurrent(MainWindow::appPath);
	QFile f("cookies.txt");
	qint64 bytesWritten;

	if (!f.open(QIODevice::ReadWrite)) {
		qDebug() << "unable to save cookies";
		return;
	}

	QListIterator<QNetworkCookie> i(allCookies());
	while(i.hasNext()) {
		bytesWritten += f.write(i.next().toRawForm());
		bytesWritten += f.write("\n");
	}

	f.flush();
	f.resize(bytesWritten);
	f.close();
}
