#pragma once

#include <QObject>
#include "Progress.h"
#include "Console.h"
#include "Service.h"


class Phpcgi : public Service
{
	Q_OBJECT

public:


	Phpcgi(QObject* parent, Console*, Path*);
	~Phpcgi();

private:
	Console* console;
	Progress* progress;
	bool init();
public slots:
	void stop();
	void start();
	void check(); //检查服务是否启动成功

};
