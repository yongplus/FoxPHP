#pragma once

#include <QProcess>

class Progress : public QProcess
{
	Q_OBJECT

public:
	Progress(QObject* parent = nullptr);
	~Progress();
};
