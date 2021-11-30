#pragma once

#include <QObject>
#include <QLabel>

class Path : public QObject
{
	Q_OBJECT


public:
	enum Item
	{
		APP,
		PHP,
		Server,
		Nginx,
		Host,
	};
	Path(QObject* parent);
	QString phpIni();
	QString ngxConf();
	QString server();
	~Path();
	QString app;
	QString php;
	QString nginx;
	QString host;

public slots:
	void open(Path::Item);

};
