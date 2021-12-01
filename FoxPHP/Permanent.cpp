#include "Permanent.h"

Permanent::Permanent(QObject* parent)
	: QObject(parent)
{
	initReg();
}

bool Permanent::isAutoRun() {
	if (reg->value("disableAutoRun").toBool()) {
		return false;
	}
	else {
		return true;
	}
}

void Permanent::setAutoRun(bool run) {
	reg->setValue("disableAutoRun", !run);
}

bool Permanent::isFirstRun() {
	if (!reg->value("run_ever").toBool()) {
		reg->setValue("run_ever", true);
		return true;
	}
	else {
		return false;
	}
}

void Permanent::initReg() {
	reg = new QSettings("lane", "FoxPHP", this);
	reg->beginGroup("Property");

}

Permanent::~Permanent()
{
	delete reg;
}
