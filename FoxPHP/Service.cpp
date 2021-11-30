#include "Service.h"

Service::Service(QObject* parent, Path* _path)
	: QObject(parent), state(State::NONE), path(_path)
{
}

void Service::setState(State state) {
	this->state = state;
	emit stateChanged(state);
}

void Service::setPath(Path* _path) {
	path = _path;
}

Service::~Service()
{
}
