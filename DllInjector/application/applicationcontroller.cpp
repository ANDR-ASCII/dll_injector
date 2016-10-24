#include "stdafx.h"
#include "applicationcontroller.hpp"
#include <windows.h>
#include <winbase.h>

namespace AppSpace
{

	void ApplicationController::setSeDebugPrivilege(bool flag)
	{
		HANDLE hThisProcess = ::GetCurrentProcess();

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

	void ApplicationController::createRemoteThread(DWORD pid, QString const& pathToDll)
	{
		emit signal_OnAboutLog(QString(80, '-'));

		HANDLE hInjectingProcess = ::OpenProcess(
			PROCESS_CREATE_THREAD |
			PROCESS_QUERY_INFORMATION |
			PROCESS_VM_OPERATION |
			PROCESS_VM_WRITE |
			PROCESS_VM_READ,
			FALSE,
			pid
		);

		if (hInjectingProcess == nullptr)
		{
			emit signal_OnAboutLog(QString("OpenProcess fails with: %1").arg(::GetLastError()));
			emit signal_OnAboutLog(QString(80, '-'));
			return;
		}

		emit signal_OnAboutLog("Process was opened...");

		std::wstring path = pathToDll.toStdWString();

		LPVOID ptrToString = 
			::VirtualAllocEx(hInjectingProcess, nullptr, (path.size() + 1) * sizeof(wchar_t), MEM_COMMIT, PAGE_READWRITE);

		if (!ptrToString)
		{
			emit signal_OnAboutLog(QString("VirtualAllocEx fails with: %1").arg(::GetLastError()));
			emit signal_OnAboutLog(QString(80, '-'));
			return;
		}

		emit signal_OnAboutLog("Memory for 'path to dll' string allocated to the specified process...");

		if (!::WriteProcessMemory(hInjectingProcess, ptrToString, path.c_str(), (path.size() + 1) * sizeof(wchar_t), nullptr))
		{
			emit signal_OnAboutLog(QString("WriteProcessMemory fails with: %1").arg(::GetLastError()));
			emit signal_OnAboutLog(QString(80, '-'));
			return;
		}

		emit signal_OnAboutLog(QString("'path to dll' string injected to the specified process by %1 address..."));

		PTHREAD_START_ROUTINE startRoutine = 
			reinterpret_cast<PTHREAD_START_ROUTINE>(::GetProcAddress(::GetModuleHandle(TEXT("kernel32.dll")), "LoadLibraryW"));

		emit signal_OnAboutLog("Injecting...");

		HANDLE hRemoteThread = ::CreateRemoteThread(hInjectingProcess, nullptr, 0, startRoutine, ptrToString, 0, nullptr);

		if (hRemoteThread == nullptr)
		{
			emit signal_OnAboutLog(QString("CreateRemoteThread fails with: %1").arg(::GetLastError()));
			emit signal_OnAboutLog(QString(80, '-'));
			return;
		}

		::WaitForSingleObject(hRemoteThread, INFINITE);
		
		::CloseHandle(hRemoteThread);
		::CloseHandle(hInjectingProcess);

		emit signal_OnAboutLog(QString("DLL was successful injected by %1 address!"));
		emit signal_OnAboutLog(QString(80, '-'));
	}

}