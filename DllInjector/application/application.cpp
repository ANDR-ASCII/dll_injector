#include "stdafx.h"
#include "application.h"

namespace Injector
{

Application::Application(int argc, char** argv, QObject* parent)
	: QObject(parent)
	, m_app(argc, argv)
	, m_appController(new ApplicationController(this))
	, m_mainFrame(new MainFrame)
{
	m_appController->setSeDebugPrivilege(true);
}

int Application::exec()
{
	return m_app.exec();
}

void Application::slot_OnAboutInject(ApplicationController::InjectionMethod method, DWORD pid, QString const& pathToDll)
{
	switch (method)
	{
	case ApplicationController::InjectionMethod::CreateRemoteThread:
		m_appController->createRemoteThread(pid, pathToDll);

	default:
		{}
	}
}

}