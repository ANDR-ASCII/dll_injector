#include "stdafx.h"
#include "dllinjector.h"
#include "common/common.hpp"
#include "common/servicelocator.hpp"
#include "application/applicationcontroller.hpp"

namespace AppSpace
{
	using namespace Common;
	//------------------------------------------------------------------------------
	DllInjector::DllInjector(QWidget *parent)
		: QMainWindow(parent)
		, m_selectProcessWindow(new SelectProcess)
		, m_processesSnapshotModel(new ProcessSnapshotModel(this))
		, m_appController(new ApplicationController(this))
	{
		init();
	}	
	//------------------------------------------------------------------------------
	void DllInjector::init()
	{
		ui.setupUi(this);

		m_selectProcessWindow->setModel(m_processesSnapshotModel);

		VERIFY(connect(ui.selectProcessButton, SIGNAL(clicked()), m_selectProcessWindow.get(), SLOT(show())));

		VERIFY(connect(ui.runButton, SIGNAL(clicked()), this, SLOT(slot_RunButtonClicked())));

		VERIFY(connect(m_selectProcessWindow.get(), SIGNAL(signal_SelectionChanged()), this, SLOT(slot_SelectProcess())));
	}
	//------------------------------------------------------------------------------
	void DllInjector::slot_RunButtonClicked()
	{
		QString processName = ui.processNameLineEdit->text();
		QString dllName = ui.dllNameLineEdit->text();

		if (processName.isEmpty() || dllName.isEmpty())
		{
			showSimpleNotification(
				"Notification", 
				"Process name and DLL name should be specified", 
				QMessageBox::Information
			);

			return;
		}

		bool createRemoteThreadSelected = ui.createRemoteThreadRadioButton->isChecked();
		bool setWindowsHookExSelected = ui.setWindowsHookExRadioButton->isChecked();
		bool windowsRegistrySelected = ui.windowsRegistryRadioButton->isChecked();

		if (createRemoteThreadSelected)
		{
			ServiceLocator* serviceLocator = ServiceLocator::instance();
			ApplicationController* appController = serviceLocator->service<ApplicationController>();

			DWORD selectedPID = m_selectProcessWindow->selectedProcessID();

			appController->createRemoteThread(selectedPID, dllName);
		}

		if (setWindowsHookExSelected)
		{
			showSimpleNotification(
				"Error",
				"Method not implemented",
				QMessageBox::Critical
			);

			return;
		}

		if (windowsRegistrySelected)
		{
			showSimpleNotification(
				"Error",
				"Method not implemented",
				QMessageBox::Critical
			);

			return;
		}
	}
	//------------------------------------------------------------------------------
	void DllInjector::slot_SelectProcess()
	{
		DWORD selectedPID = m_selectProcessWindow->selectedProcessID();
		QString selectedProcessName = m_selectProcessWindow->selectedProcessName();

		ui.processNameLineEdit->setText(QString("[%1] %2").arg(selectedPID).arg(selectedProcessName));
	}
	//------------------------------------------------------------------------------
}