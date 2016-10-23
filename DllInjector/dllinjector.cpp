#include "stdafx.h"
#include "dllinjector.h"
#include "common/common.hpp"
#include "common/servicelocator.hpp"
#include "application/applicationcontroller.hpp"

namespace AppSpace
{
	using namespace Common;

	DllInjector::DllInjector(QWidget *parent)
		: QMainWindow(parent)
		, m_selectProcessWindow(new SelectProcess)
		, m_processesSnapshotModel(new ProcessSnapshotModel(this))
		, m_appController(new ApplicationController(this))
	{
		init();
	}	
	
	void DllInjector::init()
	{
		ui.setupUi(this);

		VERIFY(connect(ui.selectProcessButton, SIGNAL(clicked()), m_selectProcessWindow.get(), SLOT(show())));
		VERIFY(connect(ui.runButton, SIGNAL(clicked()), this, SLOT(slot_RunButtonClicked())));
	}

	void DllInjector::slot_RunButtonClicked()
	{
		QString processName = ui.processNameLineEdit->text();
		QString dllName = ui.dllNameLineEdit->text();

		if (processName.isEmpty() || dllName.isEmpty())
		{
			slot_ShowErrorMessageBox("Process name and DLL name should be specified");
			return;
		}

		bool createRemoteThreadSelected = ui.createRemoteThreadRadioButton->isChecked();
		bool setWindowsHookExSelected = ui.setWindowsHookExRadioButton->isChecked();
		bool windowsRegistrySelected = ui.windowsRegistryRadioButton->isChecked();

		if (createRemoteThreadSelected)
		{
			ServiceLocator* serviceLocator = ServiceLocator::instance();
			ApplicationController* appController = serviceLocator->service<ApplicationController>();



			//appController->createRemoteThread(processName, dllName);
		}

		if (setWindowsHookExSelected)
		{
			slot_ShowErrorMessageBox("Method not implemented");
			return;
		}

		if (windowsRegistrySelected)
		{
			slot_ShowErrorMessageBox("Method not implemented");
			return;
		}
	}

	void DllInjector::slot_ShowErrorMessageBox(QString const& message)
	{
		QMessageBox errorBox;
		errorBox.setWindowTitle("Error");
		errorBox.setText(message);
		errorBox.exec();
	}

}