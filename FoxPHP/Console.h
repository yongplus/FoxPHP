#pragma once

#include <QObject>
#include <QTextBrowser>

class Console : public QTextBrowser
{
	Q_OBJECT

public:

	Console(QWidget* parent);
	~Console();
	void info(const QString& text);
	void error(const QString& text);
private:
	QTextBrowser* widget;
protected:
	virtual void contextMenuEvent(QContextMenuEvent*) override;
};
