#pragma once

#include <QObject>
#include <QThread>
#include <QPushButton>
#include "Service.h"

class Executor : public QObject
{
	Q_OBJECT

private:
	QThread* startThread;
	QThread* stopThread;
	QPushButton* btnLaunch;
	QPushButton* btnStop;

public:
	Service* service;
	Executor(QObject*, Service*, QPushButton*, QPushButton*);
	~Executor();

public slots:
	void start();
	void stop();
	void stateChanged(Service::State state);
};
