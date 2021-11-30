#pragma once

#include <QSystemTrayIcon>
#include <QMainWindow>

class SystemTray : public QSystemTrayIcon
{
	Q_OBJECT

public:
	SystemTray(QMainWindow*);
	~SystemTray();
	void online(bool);
	void offline();
private:
	QMainWindow* window;
	void initMenu();

public slots:

	void activeTray(QSystemTrayIcon::ActivationReason);
signals:
	void exit();
};
