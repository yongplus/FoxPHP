#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include "SystemTray.h"
#include <QMenu>
#include <QAction>

SystemTray::SystemTray(QMainWindow* _window)
	: QSystemTrayIcon(_window), window(_window)
{

	this->setToolTip("Fox服务");
	this->offline();
	this->initMenu();
	this->show();

	connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(activeTray(QSystemTrayIcon::ActivationReason)));
}

void SystemTray::online(bool all) {
	if (all) { //所有服务都起来
		this->setIcon(QIcon(":/images/res/icon.ico"));
	}
	else {
		this->setIcon(QIcon(":/images/res/icon_blue.png"));
	}

}

void SystemTray::offline() {
	this->setIcon(QIcon(":/images/res/icon_gray.png"));
}

void SystemTray::initMenu() {
	QMenu* menu = new QMenu(this->window);

	menu->addSection(QString("状态:%1").arg("运行中"));
	menu->addSeparator();
	//QAction* start = new QAction(QIcon(":/images/res/exit.png"), "启 动");
	//QAction* restart = new QAction(QIcon(":/images/res/restart.png"), "重 启");
	QAction* exitAction = new QAction(QIcon(":/images/res/exit.png"), "退 出");

	//restart->setDisabled(true);

	//menu->addAction(start);
	//menu->addAction(restart);
	menu->addAction(exitAction);
	connect(exitAction, &QAction::triggered, [=]() {
		emit exit();
		});
	this->setContextMenu(menu);
	QObject::connect(menu, &QMenu::aboutToShow, [=]() {
		menu->activateWindow();
		});
}

void SystemTray::activeTray(QSystemTrayIcon::ActivationReason reason) {
	switch (reason)
	{
	case QSystemTrayIcon::Unknown:
		break;
	case QSystemTrayIcon::Context:
		break;
	case QSystemTrayIcon::DoubleClick:
		this->window->activateWindow();
		this->window->show();
		break;
	case QSystemTrayIcon::Trigger:
		break;
	case QSystemTrayIcon::MiddleClick:
		break;
	default:
		break;
	}
}

SystemTray::~SystemTray()
{
}
