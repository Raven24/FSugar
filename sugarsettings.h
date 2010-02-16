#ifndef SUGARSETTINGS_H
#define SUGARSETTINGS_H

#include <QSettings>

class SugarSettings : public QObject
{
Q_OBJECT
public:
	SugarSettings(QObject *parent = 0);

	static SugarSettings *getInstance();

	QString sugarHost, sugarPath;
	bool useSsl;
	QSettings *m_settings;

private:
	static SugarSettings *instance;
};

#endif // SUGARSETTINGS_H
