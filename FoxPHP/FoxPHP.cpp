#include "FoxPHP.h"
#include <qDebug>
#include <QThread>
#include <QCloseEvent>
#include <QLabel>
#include <QTimer>
#include <QDesktopServices>

FoxPHP::FoxPHP(QWidget* parent)
	: QMainWindow(parent), exit(false), path(nullptr)
{
	ui.setupUi(this);
	setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);

	this->regHostKeys();

	ui.tabWidget->setCurrentIndex(0);
	this->systemTray = new SystemTray(this);
	this->initService();
	this->setLabelsText();
	this->bindEevents();


}

void FoxPHP::initService() {
	path = new Path(this);
	Phpcgi* _phpcgi = new Phpcgi(this, ui.console, path);
	this->phpcgi = new Executor(this, _phpcgi, ui.PHPBtnLaunch, ui.PHPBtnStop);

	Nginx* _nginx = new Nginx(this, ui.console, path);
	this->nginx = new Executor(this, _nginx, ui.NGXBtnLaunch, ui.NGXBtnStop);

	this->host = new Host(this, ui.console, this->systemTray, path);



}

void FoxPHP::bindEevents() {
	connect(ui.PHPBtnLaunch, SIGNAL(clicked(bool)), this->phpcgi, SLOT(start()));
	connect(ui.PHPBtnStop, SIGNAL(clicked(bool)), this->phpcgi, SLOT(stop()));


	connect(ui.NGXBtnLaunch, SIGNAL(clicked(bool)), this->nginx, SLOT(start()));
	connect(ui.NGXBtnStop, SIGNAL(clicked(bool)), this->nginx, SLOT(stop()));

	connect(ui.HOSTBtnEnable, SIGNAL(clicked(bool)), this->host, SLOT(enable()));
	connect(ui.HOSTBtnDisable, SIGNAL(clicked(bool)), this->host, SLOT(disable()));

	connect(ui.ngxLabel, &QPushButton::clicked, [=]() {
		path->open(Path::Item::Nginx);
		});

	connect(ui.phpLabel, &QPushButton::clicked, [=]() {
		path->open(Path::Item::PHP);
		});
	connect(ui.hostLabel, &QPushButton::clicked, [=]() {
		path->open(Path::Item::Host);
		});
	connect(ui.appLabel, &QPushButton::clicked, [=]() {
		path->open(Path::Item::APP);
		});
	connect(ui.serverLabel, &QPushButton::clicked, [=]() {
		path->open(Path::Item::Server);
		});
	connect(ui.autoRunCheckBox, &QCheckBox::stateChanged, [=]() {
		Permanent(this).setAutoRun(ui.autoRunCheckBox->isChecked());
		});

	connect(this->phpcgi->service, SIGNAL(stateChanged(Service::State)), this, SLOT(stateChanged(Service::State)));
	connect(this->nginx->service, SIGNAL(stateChanged(Service::State)), this, SLOT(stateChanged(Service::State)));


	connect(this->systemTray, &SystemTray::exit, [=]() {
		this->exit = true;
		this->close();
		});

	if (Permanent(this).isAutoRun()) {
		ui.autoRunCheckBox->setChecked(true);
		QTimer* timer = new QTimer();
		timer->setSingleShot(true);
		connect(timer, SIGNAL(timeout()), this, SLOT(autoLuanch()));
		timer->start(100);
	}
}




void FoxPHP::setLabelsText() {
	QString phppath = this->path->phpIni();
	QString ngxconf = this->path->ngxConf();
	QString server = this->path->server();
	ui.phpLabel->setText(phppath != NULL ? phppath : "未知路径");
	ui.ngxLabel->setText(ngxconf != NULL ? ngxconf : "未知路径");
	ui.hostLabel->setText(path->host != NULL ? path->host : "未知路径");
	ui.appLabel->setText(path->app != NULL ? path->app : "未知路径");
	ui.serverLabel->setText(server != NULL ? server : "未知路径");
	ui.autoRunCheckBox->setChecked(Permanent(this).isAutoRun());

}



void FoxPHP::closeEvent(QCloseEvent* event) {
	if (!this->exit) {
		this->hide();
		event->ignore();
		return;
	}
	this->phpcgi->stop();
	this->nginx->stop();
	this->systemTray->hide();
	event->accept();
}

bool FoxPHP::nativeEvent(const QByteArray& eventType, void* message, long* result)
{
	Q_UNUSED(eventType);
	Q_UNUSED(result);
	MSG* msg = static_cast<MSG*>(message);
	if (msg->message == WM_HOTKEY)
	{
		qDebug() << msg->lParam << msg->wParam;
		switch (msg->wParam)
		{
		case 1100:
			this->host->disable();
			break;
		case  1101:
			this->host->enable();
			break;
		default:
			break;
		}
		qDebug() << "Hotkey pressed!";
		true;
	}
	return false;
}

void FoxPHP::regHostKeys() {
	if (!RegisterHotKey(HWND(winId()), 1100, MOD_CONTROL | MOD_ALT, 0x44))
	{
		qDebug() << "注册快捷键失败";
	}

	if (!RegisterHotKey(HWND(winId()), 1101, MOD_CONTROL | MOD_ALT, 0x45))
	{
		qDebug() << "注册快捷键失败";
	}

}

void FoxPHP::autoLuanch() {
	this->phpcgi->start();
	this->nginx->start();
}

void FoxPHP::stateChanged(Service::State state) {
	bool php = this->phpcgi->service->state == Service::State::RUNNING;
	bool nginx = this->nginx->service->state == Service::State::RUNNING;
	if (php || nginx) {
		this->systemTray->online(php && nginx);

		if (php && nginx && Permanent(this).isFirstRun()) {
			QDesktopServices::openUrl(QUrl("http://localhost/"));
		}
	}
	else {
		this->systemTray->offline();
		qDebug() << "online";
	}
}

FoxPHP::~FoxPHP() {
	delete this->phpcgi;
	delete this->nginx;
	delete this->systemTray;
	//this->nginx->stop(true);
	//this->phpcgi->stop(true);

}
