#ifndef COOKIEJAR_H
#define COOKIEJAR_H

#include <QNetworkCookieJar>

class CookieJar : public QNetworkCookieJar
{
	Q_OBJECT

public:
	CookieJar(QObject* parent = 0);
	~CookieJar();

private:
	void populateCookies();
	void saveCookies();
};

#endif // COOKIEJAR_H
