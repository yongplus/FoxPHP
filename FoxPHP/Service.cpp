#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif


#include "Service.h"
#include <QFileInfo>
#include <qDebug>
#include <QRegularExpression>

Service::Service(QObject* parent, Path* _path, Console* _console)
	: QObject(parent), state(Service::State::NONE), path(_path), console(_console), progress(nullptr)
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

void Service::portOwner(int port) {

	QStringList params;
	Progress cmd;
	params << "/c" << "netstat" << "-nao" << "|" << "findstr" << "LISTENING" << "|" << "findstr" << QString("/C:\":%1 \"").arg(port);
	qDebug() << progress;
	cmd.setNativeArguments(params.join(" "));
	cmd.start("cmd.exe");
	cmd.waitForFinished();
	QString s(cmd.readAllStandardOutput());
	cmd.close();

	QStringList lines = s.split("\r\n");
	if (lines.size() == 0) {
		return;
	}


	QRegularExpression re("\\s+");

	for (int i = 0; i < lines.size(); i++) {
		QString line = lines[i];
		QStringList seg = line.trimmed().split(re);
		if (seg.size() != 5) {
			continue;
		}

		if (seg.at(1).split(":").last() != QString("%1").arg(port)) {
			continue;
		}

		Progress cmd;
		cmd.setNativeArguments(QString("/c tasklist /fi \"pid eq %1\"").arg(seg.last()));
		cmd.start("cmd.exe");
		cmd.waitForFinished();
		QString s(cmd.readAllStandardOutput());
		cmd.close();
		QStringList lines = s.trimmed().split("\r\n");
		if (lines.size() < 3) {
			return;
		}
		QStringList arr = lines.at(2).trimmed().split(re);
		if (arr.size() < 3) {
			return;
		}
		console->error(QString("端口%1被%2进程占用，PID：%3").arg(port).arg(arr.at(0)).arg(seg.last()));
		return;
	}






}

Service::~Service()
{
}
