#include "stdafx.h"
#include "dllinjector.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	DllInjector w;
	w.show();
	return a.exec();
}
