#pragma once

#include "applicationcontroller.h"
#include "../widgets/mainframe.h"
#include "../common/common.h"
#include <memory>
#include <QApplication>
#include <QObject>

namespace Injector
{

class Application : public QObject
{
	Q_OBJECT

public:
	Application(int argc, char** argv, QObject* parent = nullptr);

	int exec();

private Q_SLOTS:
	void slot_OnAboutInject(ApplicationController::InjectionMethod method, DWORD pid, QString const& pathToDll);

private:
	QApplication m_app;

	ApplicationController* m_appController;

	std::unique_ptr<MainFrame> m_mainFrame;
};

}
