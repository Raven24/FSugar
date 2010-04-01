#ifndef SUGARSETTINGS_H
#define SUGARSETTINGS_H

#include <QSettings>

class SugarSettings : public QObject
{
Q_OBJECT
public:
	SugarSettings(QObject *parent = 0);

	static SugarSettings *getInstance();
	void makeUpdate();

	QString sugarHost, sugarPath, calendarUrl,
			sugarUser, sugarPass;
	bool useSsl;
	QSettings *m_settings;

signals:
	void settingsChanged();

public slots:
	void assignVars();

private:
	static SugarSettings *instance;

};

#endif // SUGARSETTINGS_H
