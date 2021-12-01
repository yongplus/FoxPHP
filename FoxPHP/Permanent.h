#pragma once

#include <QObject>
#include <QSettings>

class Permanent : public QObject
{
	Q_OBJECT

public:
	Permanent(QObject* parent);
	bool isAutoRun();
	void setAutoRun(bool);
	bool isFirstRun();
	~Permanent();

private:
	QSettings* reg;
	void initReg();
};
