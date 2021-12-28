#include "Service.h"
#include <QFileInfo>
#include "Progress.h"

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

void Service::kill(const QString& exec) {
	QString filename = QFileInfo(exec).fileName();
	QStringList params;
	params << "/c" << "taskkill" << "-f" << "-im" << filename;
	Progress killer;
	killer.start("cmd.exe", params);
	killer.waitForFinished();
	killer.close();
}
Service::~Service()
{
}
