#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include "Host.h"
#include <qDebug>
#include <QRegularExpression>

Host::Host(QObject* parent, Console* _console, SystemTray* _systemTray, Path* _path)
	: QObject(parent), console(_console), systemTray(_systemTray), path(_path)
{
	beginFlag = "#BeginFoxJKY";
	endFlag = "#EndFoxJKY";
}

void Host::enable() {
	QString text = read();
	if (text == NULL) {
		return;
	}

	int startPos = text.indexOf(beginFlag);
	qDebug() << startPos;
	if (startPos > -1) {
		int endPos = text.indexOf(endFlag, startPos);
		if (endPos == -1) {
			tip("查找EndFoxJKY结束标签失败~！", true);
			return;
		}
		startPos += beginFlag.length();

		QStringRef subString(&text, startPos, endPos - startPos);
		QString range = subString.toString();

		QRegularExpression re("^#\\s+?(.*)$", QRegularExpression::MultilineOption);
		text = text.left(startPos) + range.replace(re, "\\1") + text.right(text.length() - endPos);
	}
	else {
		text.append("\n" + beginFlag + "\n"
			+ "127.0.0.1 xx.com\n"
			+ endFlag);
	}
	if (write(text)) {
		tip("启用host成功");
	}



}

void Host::disable() {
	QString text = this->read();
	if (text == NULL) {
		return;
	}

	int startPos = text.indexOf(beginFlag);

	if (startPos > -1) {

		int endPos = text.indexOf(endFlag, startPos);
		startPos += beginFlag.length() + 1;

		QStringRef subString(&text, startPos, endPos - startPos);
		QString range = subString.toString();

		QRegularExpression re("^([^#].+)$", QRegularExpression::MultilineOption);
		text = text.left(startPos) + range.replace(re, "# \\1") + text.right(text.length() - endPos);
		if (write(text)) {
			tip("禁用host成功");
		}
	}
	else {
		tip("未检测到相关host", true);
	}


}

QString Host::read() {
	QFile file(path->host);

	if (!file.exists()) {
		tip("启用HOST失败，HOST文件不存在~！", true);
		return NULL;
	}

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		tip("打开host文件出错！" + file.errorString(), true);
		return NULL;
	}

	QString text = file.readAll();
	if (text == NULL) {
		text = " ";
	}
	file.close();
	return text;
}


void Host::tip(const QString& text, bool error) {

	if (error) {
		this->console->error(text);
	}
	else {
		this->console->info(text);
	}

	QMainWindow* window = qobject_cast<QMainWindow*>(this->parent());
	if (!window->isActiveWindow()) {
		this->systemTray->showMessage("提示", text, error ? QSystemTrayIcon::Warning : QSystemTrayIcon::Information, 3000);
	}
}



bool Host::write(const QString& text) {
	QFile file(path->host);

	if (!file.exists()) {
		this->console->error("启用HOST失败，HOST文件不存在~！");
		return false;
	}

	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		this->console->error("打开host文件出错！" + file.errorString());
		return false;
	}
	file.write(text.toUtf8());
	file.flush();
	file.close();
	return true;
}

Host::~Host()
{
}
