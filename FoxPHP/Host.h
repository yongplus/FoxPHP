#pragma once

#include <QObject>
#include <QFile>
#include "Console.h"
#include "SystemTray.h"
#include "Path.h"

class Host : public QObject
{
	Q_OBJECT

private:
	Path* path;
	Console* console;
	SystemTray* systemTray;
	QString beginFlag;
	QString endFlag;
	QString read();
	bool write(const QString&);
	void tip(const QString&, bool error = false);

public:
	Host(QObject*, Console*, SystemTray*, Path*);
	~Host();

public slots:
	void enable();
	void disable();
};
