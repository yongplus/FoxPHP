#include "FoxPHP.h"
#include "SingleApplication.h"

int main(int argc, char* argv[])
{
	SingleApplication a(argc, argv);
	if (a.isRunning()) {
		return 0;
	}

	QFont f("Helvetica", 9);
	a.setFont(f);

	FoxPHP w;
	a.setSingleMainWindow(&w);
	w.show();
	return a.exec();
}
