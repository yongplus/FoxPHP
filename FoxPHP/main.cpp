#include "FoxPHP.h"
#include "SingleApplication.h"

int main(int argc, char* argv[])
{
	SingleApplication a(argc, argv);
	if (a.isRunning()) {
		return 0;
	}

	QFont font("NSimSun");
	font.setPixelSize(12);
	a.setFont(font);

	FoxPHP w;
	a.setSingleMainWindow(&w);
	w.show();
	return a.exec();
}
