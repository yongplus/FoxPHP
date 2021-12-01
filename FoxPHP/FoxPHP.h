#pragma once

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include <QtWidgets/QMainWindow>
#include "ui_FoxPHP.h"
#include "Phpcgi.h"
#include "Nginx.h"
#include "SystemTray.h"
#include "Executor.h"
#include "Host.h"
#include "Path.h"
#include "Permanent.h"
#include "windows.h"


class FoxPHP : public QMainWindow
{
	Q_OBJECT


public:
	FoxPHP(QWidget* parent = Q_NULLPTR);
	~FoxPHP();

	Executor* phpcgi;
	Executor* nginx;
	Host* host;
	Path* path;
	void closeEvent(QCloseEvent* event);
	bool nativeEvent(const QByteArray& eventType, void* message, long* result);

private:
	bool exit;
	SystemTray* systemTray;
	void regHostKeys();
	void initService();
	void bindEevents();
	void setLabelsText();
	Ui::FoxPHPClass ui;

public slots:
	void autoLuanch();
	void stateChanged(Service::State state);

};
