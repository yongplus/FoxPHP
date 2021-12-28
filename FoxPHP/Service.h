#pragma once

#include <QObject>
#include "Path.h"

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
	Service(QObject* parent, Path*);
	~Service();
	void setPath(Path*);
	void Service::kill(const QString&);

public slots:
	//启动
	virtual void start() = 0;
	//停止
	virtual void stop() = 0;
	void setState(State);

protected:
	virtual bool init() = 0;
	Path* path;
signals:
	void stateChanged(Service::State);
};
