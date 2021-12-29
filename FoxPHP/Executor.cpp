#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif



#include "Executor.h"
#include <qDebug>
#include <QEvent>
#include <QApplication>

Executor::Executor(QObject* parent, Service* _service, QPushButton* _btnLaunch, QPushButton* _btnStop)
	: QObject(parent), service(_service)
{
	this->startThread = new QThread(this);
	this->stopThread = new QThread(this);
	this->btnLaunch = _btnLaunch;
	this->btnStop = _btnStop;

	connect(this->startThread, SIGNAL(started()), service, SLOT(start()));
	connect(this->stopThread, SIGNAL(started()), service, SLOT(stop()));

	connect(service, SIGNAL(stateChanged(Service::State)), this, SLOT(stateChanged(Service::State)));
}

void Executor::start() {
	this->startThread->quit();
	this->startThread->wait();
	this->startThread->start();
}

void Executor::stop() {

	this->stopThread->quit();
	this->stopThread->wait();
	this->stopThread->start();
}

void Executor::stateChanged(Service::State state) {
	qDebug() << "收到事件" << state;

	if (state == Service::State::PENDING) {
		btnLaunch->setDisabled(true);
		//QEvent* leaveEvent = new QEvent(QEvent::Leave);
		//QApplication::sendEvent(btnLaunch, leaveEvent);
		btnLaunch->setAttribute(Qt::WA_UnderMouse, false);
	}
	else if (state == Service::State::RUNNING) {
		btnStop->setDisabled(false);
	}
	else {
		btnLaunch->setDisabled(false);
		btnStop->setDisabled(true);
		btnStop->setAttribute(Qt::WA_UnderMouse, false);
	}
}


Executor::~Executor()
{
	this->startThread->quit();
	this->startThread->wait();
	this->startThread->deleteLater();
	this->stopThread->quit();
	this->stopThread->wait();
	this->stopThread->deleteLater();
}
