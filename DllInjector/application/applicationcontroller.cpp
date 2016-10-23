#include "stdafx.h"
#include "applicationcontroller.hpp"
#include <windows.h>
#include <winbase.h>

namespace AppSpace
{

	void ApplicationController::setSeDebugPrivilege(bool flag)
	{
		HANDLE hThisProcess = ::OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, ::GetCurrentProcessId());

		if (hThisProcess == nullptr)
		{
			return;
		}

		HANDLE hThisProcessToken = nullptr;

		if (!::OpenProcessToken(hThisProcess, TOKEN_ADJUST_PRIVILEGES, &hThisProcess))
		{
			return;
		}

		TOKEN_PRIVILEGES tokenPrivileges;
		LUID luid;

		BOOL bLookupResult = 
			::LookupPrivilegeValue(nullptr, SE_DEBUG_NAME, &luid);

		if (!bLookupResult)
		{
			return;
		}

		tokenPrivileges.PrivilegeCount = 1;
		tokenPrivileges.Privileges[0].Luid = luid;

		DWORD enableStatus = flag ? SE_PRIVILEGE_ENABLED : SE_PRIVILEGE_REMOVED;

		tokenPrivileges.Privileges[0].Attributes = enableStatus;

		BOOL bAdbjustResult = 
			::AdjustTokenPrivileges(hThisProcessToken, FALSE, &tokenPrivileges, sizeof(tokenPrivileges), nullptr, nullptr);

		if (!bAdbjustResult || ::GetLastError() == ERROR_NOT_ALL_ASSIGNED)
		{
			return;
		}

		m_seDebugPrivilege = flag;
	}

	bool ApplicationController::seDebugPrivilege() const
	{
		return m_seDebugPrivilege;
	}

	bool ApplicationController::createRemoteThread(DWORD pid, QString const& dllName)
	{
		//HANDLE hInjectingProcess = ::OpenProcess
		return false;
	}

}