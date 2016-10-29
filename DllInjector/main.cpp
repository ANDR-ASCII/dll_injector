#include "stdafx.h"
#include "widgets/mainframe.h"
#include "application/application.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	Injector::ApplicationController appController(argc, argv);
	return appController.exec();
}
