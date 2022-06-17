#pragma once

#include <QObject>
#include "Progress.h"
#include "Service.h"
#include "Console.h"
#include "Path.h"

class Nginx : public Service
{
	Q_OBJECT

public:
	QString program;
	Nginx(QObject* parent, Console*, Path*);
	~Nginx();

private:
	bool init();
public slots:
	void start();
	void stop();
	void check();

};
