#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include "Phpcgi.h"
#include <qDebug>
#include <QTimer>
#include <QApplication>
#include <QFileInfo>
#include <QDir>


Phpcgi::Phpcgi(QObject* parent, Console* _console, Path* _path)
	: Service(parent, _path), console(_console), progress(nullptr)
{


}

bool Phpcgi::init() {
	if (progress != nullptr) {
		return true;
	}

	if (path->php == NULL) {
		this->console->error("php-cgi服务路径不正确");
		return false;
	}

	QString program = path->php;
	QStringList arguments;

	arguments << "-b" << "127.0.0.1:9000";
	//arguments << "-b" << "123.56.6.2:9000";

	progress = new Progress(this);
	progress->setWorkingDirectory(QFileInfo(program).absoluteDir().path());


	progress->setProgram(program);
	progress->setArguments(arguments);

	connect(progress, &QProcess::errorOccurred, [=](QProcess::ProcessError error) {
		setState(State::EXIT);
		});
	connect(progress, &QProcess::started, [=]() {
		QTimer* timer = new QTimer();
		timer->setSingleShot(true);
		connect(timer, SIGNAL(timeout()), this, SLOT(check()));
		timer->start(1000);
		});

	connect(progress, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [=](int exitCode, QProcess::ExitStatus exitStatus) {
		qDebug() << exitCode << exitStatus;
		if (exitCode == 0 || exitCode == 62097 || state == State::RUNNING) {
			console->info("PHP-CGI停止成功!");
		}
		else if (exitCode == -1) {
			QString error(progress->readAllStandardError());
			console->error(QString("启动PHP-CGI错误：\n%1").arg(error));
		}
		setState(State::STOP);
		});
	return true;
}

void Phpcgi::start() {

	if (state == State::RUNNING || !this->init()) {
		return;
	}

	console->info("PHP-CGI启动中...");
	kill(path->php);
	setState(State::PENDING);
	progress->start();
}


void Phpcgi::stop() {
	if (state != State::RUNNING) {
		return;
	}
	console->info("PHP-CGI正在停止!");
	progress->kill();
	progress->waitForFinished();
}



void Phpcgi::check() {

	if (state == State::PENDING) {
		console->info("PHP-CGI启动成功!");
		QString warning(progress->readAllStandardError());
		if (!warning.isEmpty()) {
			console->error(warning);
		}
		setState(State::RUNNING);
	}
}




Phpcgi::~Phpcgi()
{
}
