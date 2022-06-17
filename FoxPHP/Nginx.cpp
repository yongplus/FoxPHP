#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif


#include "Nginx.h"
#include <qDebug>
#include <QFileInfo>
#include <QDir>
#include <QTimer>

Nginx::Nginx(QObject* parent, Console* _console, Path* _path)
	: Service(parent, _path, _console)
{

}

bool Nginx::init() {
	if (progress != nullptr) {
		return true;
	}
	if (path->nginx == NULL) {
		this->console->error("nginx服务路径不正确");
		return false;
	}

	progress = new Progress(this);
	progress->setWorkingDirectory(QFileInfo(path->nginx).absoluteDir().path());
	progress->setProgram(path->nginx);

	QStringList arguments;
	//arguments << "-h";
	progress->setArguments(arguments);

	connect(progress, &QProcess::errorOccurred, [=](QProcess::ProcessError error) {
		QString output(progress->readAllStandardError());
		qDebug() << output;

		setState(State::EXIT);
		});
	connect(progress, &QProcess::started, [=]() {
		QTimer* timer = new QTimer();
		timer->setSingleShot(true);
		connect(timer, SIGNAL(timeout()), this, SLOT(check()));
		timer->start(1000);
		});

	connect(progress, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [=](int exitCode, QProcess::ExitStatus exitStatus) {

		if (state == State::PENDING) {

			QString error(progress->readAllStandardError());
			if (!error.isEmpty()) {
				console->error(QString("Nginx启动错误:\n%1").arg(error));
			}
		}
		else if (state == State::RUNNING) {
			console->info("Nginx停止成功!");
		}


		setState(State::STOP);
		});
	return true;
}

void Nginx::start() {

	if (state == State::RUNNING || !init()) {
		return;
	}



	console->info("Nginx启动中...");
	kill(path->nginx); //启动前先kill之前的进程
	setState(State::PENDING);
	progress->start();

}








void Nginx::stop() {
	if (state != State::RUNNING) {
		return;
	}


	Progress* stoper = new Progress(this);
	QStringList arguments;
	arguments << "-s" << "stop";
	stoper->setWorkingDirectory(QFileInfo(path->nginx).absoluteDir().path());
	stoper->start(path->nginx, arguments);
	console->info("Nginx正在停止!");
	stoper->waitForFinished();
	if (stoper->exitCode() == 1) {
		QString error(stoper->readAllStandardError());
		console->error(QString("停止Nginx失败：\n%1").arg(error));
	}
	stoper->close();
}


void Nginx::check() {
	if (state == State::PENDING) {
		qDebug() << progress->readAll();
		qDebug() << progress->readAllStandardError();
		qDebug() << progress->readAllStandardOutput();
		console->info("Nginx启动成功!");
		setState(State::RUNNING);
	}
}




Nginx::~Nginx()
{
}
