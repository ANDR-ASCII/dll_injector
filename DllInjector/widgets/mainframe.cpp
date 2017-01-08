#include "stdafx.h"
#include "MainFrame.h"
#include "common/common.h"
#include "common/servicelocator.h"
#include "application/applicationcontroller.h"

namespace Injector
{

using namespace Common;
//------------------------------------------------------------------------------
MainFrame::MainFrame(QWidget *parent)
	: QMainWindow(parent)
	, m_selectProcessWindow(new SelectProcess(this))
{
	init();
}	
//------------------------------------------------------------------------------
void MainFrame::init()
{
	ui.setupUi(this);

	m_selectProcessWindow->hide();
	m_selectProcessWindow->setWindowFlags(Qt::Window);
	m_selectProcessWindow->setWindowIcon(QIcon("icons/Malware-icon.png"));

	VERIFY(connect(ui.selectProcessButton, SIGNAL(clicked()), this, SLOT(slot_ProcessSelectionChanged())));

	VERIFY(connect(ui.runButton, SIGNAL(clicked()), this, SLOT(slot_RunButtonClicked())));

	VERIFY(connect(m_selectProcessWindow, SIGNAL(signal_SelectionChanged()), this, SLOT(slot_SelectProcess())));

	VERIFY(connect(ui.selectDllButton, SIGNAL(clicked()), this, SLOT(slot_ShowFileDialog())));
}
//------------------------------------------------------------------------------
void MainFrame::slot_RunButtonClicked()
{	
	DisablerWidgetGuard disabler(this);

	ApplicationController::ParametersPackForInject pack;

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

	pack.pid = m_selectProcessWindow->selectedProcessID();
	pack.pathToDll = dllName;

	if (ui.createRemoteThreadRadioButton->isChecked())
	{
		pack.method = ApplicationController::CreateRemoteThreadMethod;
	}

	if (ui.createProcessRadioButton->isChecked())
	{
		pack.method = ApplicationController::CreateProcessMethod;
		pack.pathToExe = ui.processNameLineEdit->text();
	}

	if (ui.setWindowsHookExRadioButton->isChecked())
	{
		pack.method = ApplicationController::SetWindowsHookExMethod;
	}

	if (ui.windowsRegistryRadioButton->isChecked())
	{
		pack.method = ApplicationController::WindowsRegistryMethod;
	}

	emit signal_StartInjection(pack);
}
//------------------------------------------------------------------------------
void MainFrame::slot_SelectProcess()
{
	DWORD selectedPID = m_selectProcessWindow->selectedProcessID();
	QString selectedProcessName = m_selectProcessWindow->selectedProcessName();

	ui.processNameLineEdit->setText(QString("[%1] %2").arg(selectedPID).arg(selectedProcessName));
}

void MainFrame::slot_ProcessSelectionChanged()
{
	if (ui.createProcessRadioButton->isChecked())
	{
		QString dllName = QFileDialog::getOpenFileName(this, "Select executable file", "", "*.exe");
		ui.processNameLineEdit->setText(dllName);
	}
	else
	{
		m_selectProcessWindow->show();
	}
}

//------------------------------------------------------------------------------
void MainFrame::slot_ShowFileDialog()
{
	QString dllName = QFileDialog::getOpenFileName(this, "Select a DLL", "", "*.dll");
	ui.dllNameLineEdit->setText(dllName);
}

void MainFrame::onAboutLogActions(QString const& str) const
{
	ui.textEdit->append(str);
}

//------------------------------------------------------------------------------
}