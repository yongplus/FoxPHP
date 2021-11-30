#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include "Console.h"
#include <QDateTime>
#include <QAction>
#include <QMenu>
#include <QContextMenuEvent>

Console::Console(QWidget* parent)
	: QTextBrowser(parent)
{

	//this->widget->removeAction(this->widget->actions().at(1));
	this->info("欢迎使用FoxPHP");
}

void Console::contextMenuEvent(QContextMenuEvent* event) {
	QMenu* menu = createStandardContextMenu();
	menu->removeAction(menu->actions()[1]);
	menu->actions()[0]->setText("复 制");
	menu->actions()[2]->setText("全 选（Ctrl+A）");

	menu->addAction("清  空", [&]() {this->clear(); });
	if (this->toPlainText().isEmpty()) {
		menu->actions()[3]->setDisabled(true);
	}
	menu->exec(event->globalPos());
	delete menu;
}

void Console::info(const QString& text) {
	QString sTime = QTime::currentTime().toString("hh:mm:ss.zzz");

	append(QString("<span>%1: %2</span>").arg(sTime).arg(text).replace("\n", "<br />"));
}

void Console::error(const QString& text) {
	QString sTime = QTime::currentTime().toString("hh:mm:ss.zzz");
	append(QString("<span style=\"color:#f60;\">%1: %2</span>").arg(sTime).arg(text).replace("\n", "<br />"));
}

Console::~Console()
{

}
