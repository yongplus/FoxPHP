#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include "Host.h"
#include <qDebug>
#include <QRegularExpression>
#include <QMap>

Host::Host(QObject* parent, Console* _console, SystemTray* _systemTray, Path* _path)
	: QObject(parent), console(_console), systemTray(_systemTray), path(_path)
{
	beginFlag = "#BeginFoxPHP";
	endFlag = "#EndFoxPHP";
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
		range = range.replace(re, "\\1");
		QString before = comment(text.left(startPos), range, false);
		QString after = comment(text.right(text.length() - endPos), range, false);

		text = before + range.replace(re, "\\1") + after;
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
		QString before = comment(text.left(startPos), range, true);
		QString after = comment(text.right(text.length() - endPos), range, true);
		range = range.replace(re, "# \\1");

		text = before + range + after;
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


QString Host::comment(QString text, QString range, bool isStrip) {
	QStringList lines = range.replace("\r", "").split("\n");
	QListIterator<QString> iterator(lines);
	QMap<QString, int> map;


	QRegularExpression reg("\\s+|	");
	QRegularExpression reg2("^#\\s+");
	while (iterator.hasNext()) {
		QString line = iterator.next();
		line = line.replace(reg2, "").trimmed();
		QStringList segs = line.split(reg);
		if (segs.length() == 2) {
			map[segs[1]] = 1;
		}
	}


	QStringList lines2 = text.split("\n");
	QListIterator<QString> iterator2(lines2);

	QString label = "#ByFoxPHP ";
	for (int i = 0; i < lines2.length(); i++) {

		QString line = lines2[i].trimmed();
		QStringList segs = line.split(reg);


		if (isStrip) {
			if (segs.length() == 3 && map.contains(segs[2])) {
				if (segs[0].indexOf(label.trimmed()) == 0) {
					lines2[i] = lines2[i].right(lines2[i].length() - label.length());
				}
			}
		}
		else {
			if (segs.length() == 2 && map.contains(segs[1])) {
				if (segs[0].indexOf("#") != 0) {
					lines2[i] = label + lines2[i];
				}
			}
		}
	}
	lines.clear();
	return lines2.join("\n");

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
