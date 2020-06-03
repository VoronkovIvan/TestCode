#include <QApplication>

#include "main_window.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	MainWindow mWindow;
	mWindow.show();

	return a.exec();
}
