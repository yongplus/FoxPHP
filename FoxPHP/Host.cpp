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
#include <QFileInfo>
#include <QDir>
#include <QEventLoop>
#include <QDnsLookup>
#include <QThreadPool>
#include <QtConcurrent>
#include "Progress.h"





Host::Host(QObject* parent, Console* _console, SystemTray* _systemTray, Path* _path)
	: QObject(parent), console(_console), systemTray(_systemTray), path(_path), pending(false)
{
	beginFlag = "#BeginFoxPHP";
	endFlag = "#EndFoxPHP";

	connect(this, SIGNAL(doEnableSignal()), this, SLOT(doEnable()));
}

void Host::enable() {
	if (pending) {
		return;
	}
	QtConcurrent::run(QThreadPool::globalInstance(), [=]() {
		this->parseNginxDomains();
		});

	//QtConcurrent::run(this, &Host::parseNginxDomains);
}
void Host::doEnable() {

	pending = false;

	QString text = read();
	if (text == NULL) {
		return;
	}

	int startPos = text.indexOf(beginFlag);
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


void Host::parseNginxDomains() {
	QString vhost = QFileInfo(path->nginx).absoluteDir().path() + "/conf/conf.d/vhost.conf";
	QFile file(vhost);
	if (!file.exists()) {
		tip("启用HOST失败，HOST文件不存在~！", true);
		pending = false;
		return;
	}

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		tip("打开host文件出错！" + file.errorString(), true);
		pending = false;
		return;
	}

	QString conf = file.readAll();

	QStringList addrs;
	int offset = 0;
	QString servername = "server_name";
	QString listen = "listen";
	QRegularExpression reg("\\s+|	");

	while (true) {
		QString port = "80";

		//解析端口
		int start = conf.indexOf(listen, offset);
		if (start != -1) {
			start += listen.length();
			int end = conf.indexOf(";", start);
			if (end != -1) {

				QString seg = conf.left(end).right(end - start).trimmed();
				qDebug() << "listen" << seg;
				QStringList listenItem = seg.split(reg);
				if (listenItem.length() > 0 && listenItem.at(0).toInt() > 0) {
					port = listenItem.at(0);
				}
			}
		}

		//解析server_name
		start = conf.indexOf(servername, offset);
		if (start == -1) {
			break;
		}
		start += servername.length();

		offset = start;

		int end = conf.indexOf(";", offset);
		if (end == -1) {
			break;
		}
		offset = end;

		QString seg = conf.left(end).right(end - start).trimmed();
		QStringList domains = seg.split(reg);

		for (int i = 0; i < domains.length(); i++) {
			QEventLoop loop;
			QDnsLookup dns(QDnsLookup::A, domains[i]);
			connect(&dns, SIGNAL(finished()), &loop, SLOT(quit()));
			dns.lookup();
			loop.exec();
			if (dns.hostAddressRecords().length() == 0) {
				continue;
			}
			qDebug() << "string" << domains[i] << dns.hostAddressRecords().at(0).value().toString();
			QString ip = dns.hostAddressRecords().at(0).value().toString();
			loop.deleteLater();
			dns.deleteLater();
			QString addr = QString("%1:%2").arg(ip, port);
			if (ip != "127.0.0.1" && addrs.contains(addr) == false) {
				addrs << addr;
			}
		}
	}
	file.close();

	emit doEnableSignal();
	qDebug() << "len" << addrs.length();
	for (int i = 0; i < addrs.length(); i++) {
		closeKeepAlive(addrs[i]);
	}

}
void Host::closeKeepAlive(QString& addr) {

	QStringList arr = addr.split(":");
	qDebug() << "addr" << addr;
	QStringList params;
	// /close <Local Address> <Local Port> <Remote Address> <Remote Port> {Process Name/ID}

	params << "/close" << "*" << "*" << arr[0] << arr[1];
	qDebug() << "close " << params;
	Progress cmd;
	cmd.start(QString("%1/%2").arg(path->app, "cports.exe"), params);
	cmd.waitForFinished();
	cmd.kill();

	/*
	qDebug() << "addr" << addr;
	QStringList params;
	params << "/c" << "netstat" << "-nao" << "|" << "findstr" << addr;

	Progress cmd;
	cmd.start("cmd.exe", params);
	cmd.waitForFinished();

	QByteArray bytes = cmd.readAllStandardOutput();
	QStringList lines = QString(bytes).split("\r\n");
	for (int i = 0; i < lines.length(); i++) {
		QString pid = lines[i].split("     ").last();
		if (pid.isEmpty() == false) {
			qDebug() << "pid" << pid;
			params.clear();
			params << "/c" << "taskkill" << "-f" << "-pid" << pid;
			Progress killer;
			killer.start("cmd.exe", params);
			killer.waitForFinished();
		}
	}
	*/

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
