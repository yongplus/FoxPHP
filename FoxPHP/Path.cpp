#include "Path.h"
#include <QApplication>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QUrl>
#include <QDesktopServices>
#include <qDebug>
#include <QProcess>

Path::Path(QObject* parent)
	: QObject(parent)
{
	app = QApplication::applicationDirPath();
	QString _php = app + "/service/php/php-cgi.exe";
	if (QFile(_php).exists()) {
		this->php = _php;
	}

	QString _nginx = app + "/service/nginx/nginx.exe";
	if (QFile(_nginx).exists()) {
		this->nginx = _nginx;
	}

	QString _host = "C:/Windows/System32/drivers/etc/hosts";
	if (QFile(_host).exists()) {
		this->host = _host;
	}
}

QString Path::phpIni() {
	QString path = QFileInfo(php).absoluteDir().path() + "/php.ini";
	if (QFile(path).exists()) {
		return path;
	}

	path = QFileInfo(php).absoluteDir().path() + "/lib/php.ini";
	if (QFile(path).exists()) {
		return path;
	}
	return NULL;
}

QString Path::ngxConf() {

	QString path = QFileInfo(nginx).absoluteDir().path() + "/conf/nginx.conf";
	if (QFile(path).exists()) {
		return path;
	}
	return NULL;
}

QString Path::server() {

	QString path = QFileInfo(nginx).absoluteDir().path() + "/conf/conf.d/vhost.conf";
	if (QFile(path).exists()) {
		return path;
	}
	return NULL;
}


void Path::open(Path::Item item) {
	QString path = NULL;
	bool isFile = true;
	if (item == Item::PHP) {
		path = phpIni();
	}
	else if (item == Item::Nginx) {
		path = ngxConf();
	}
	else if (item == Item::APP) {
		path = app;
		isFile = false;
	}
	else if (item == Item::Server) {
		path = server();
	}
	else {
		path = this->host;
	}

	if (path == NULL) {
		return;
	}



	if (isFile && notepad(path)) {
		return;
	}


	QUrl url = QUrl("file:///" + path, QUrl::TolerantMode);
	QDesktopServices::openUrl(url);
}


void Path::findNotepad() {
	if (!notepadLocal.isEmpty()) {
		return;
	}
	QStringList _list = QStandardPaths::standardLocations(QStandardPaths::DesktopLocation);
	QString desktop = "";

	if (_list.length() > 0) {
		desktop = _list.at(0);
	}


	QStringList paths;


	paths << "C:\\ProgramData\\Microsoft\\Windows\\Start Menu\\Programs\\Notepad++.lnk";
	paths << "C:\\ProgramData\\Microsoft\\Windows\\Start Menu\\Programs\\Notepad++\\Notepad++.lnk";
	paths << "C:\\Program Files (x86)\\Notepad++\\Notepad++.exe";
	paths << "C:\\ProgramData\\Microsoft\\Windows\\Start Menu\\Programs\\Sublime Text.lnk";
	paths << "C:\\Program Files\\Notepad++\\Notepad++.exe";
	paths << "C:\\Program Files\\EditPlus\\EditPlus.exe";
	paths << "C:\\Program Files (x86)\\EditPlus\\EditPlus.exe";
	paths << "C:\\Program Files\\Sublime Text\\sublime_text.exe";
	paths << desktop + "/Sublime Text.lnk";
	paths << desktop + "/Notepad++.lnk";
	paths << desktop + "/EditPlus.lnk";

	QListIterator<QString> iterator(paths);
	while (iterator.hasNext()) {
		QString _path = iterator.next();
		QFileInfo file(_path);
		if (file.exists()) {
			if (file.isShortcut()) {
				if (QFileInfo(file.symLinkTarget()).exists()) {
					notepadLocal = file.symLinkTarget();
					break;
				}
			}
			else {
				notepadLocal = _path;
				break;
			}
		}
	}
	if (notepadLocal.isEmpty()) {
		notepadLocal = "unknow";
	}
}

bool Path::notepad(const QString& filepath) {


	findNotepad();
	if (notepadLocal == "unknow") {
		return false;
	}
	else {
		QProcess proc;
		proc.startDetached(notepadLocal, QStringList() << filepath);
		return true;
	}
}

Path::~Path()
{
}
