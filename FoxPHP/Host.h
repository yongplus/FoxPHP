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
	/*
	* 处理多个host，range确保是未注释的
	*/
	QString comment(QString text, QString range, bool isStrip = true);
public:
	Host(QObject*, Console*, SystemTray*, Path*);
	~Host();

public slots:
	void enable();
	void disable();
};
