#pragma once

#include <QObject>
#include "Path.h"
#include "Console.h"
#include "Progress.h"

class Service : public QObject
{
	Q_OBJECT

public:
	enum State {
		NONE, //无状态
		PENDING, //启动中
		RUNNING, //运行中
		STOP, //停止
		EXIT, //无法启动
	};

	State state;
	Service(QObject* parent, Path*, Console*);
	~Service();

	void setPath(Path*);
	void Service::kill(const QString&);

public slots:
	//启动
	virtual void start() = 0;
	//停止
	virtual void stop() = 0;
	void setState(State);
	void portOwner(int);

protected:
	Console* console;
	Progress* progress;
	virtual bool init() = 0;
	Path* path;
signals:
	void stateChanged(Service::State);
};
