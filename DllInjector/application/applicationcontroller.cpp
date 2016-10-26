#include "stdafx.h"
#include "applicationcontroller.hpp"
#include "common/common.hpp"
#include <fstream>
#include <cstdint>
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

		Common::Win32::HandleRAIIWrapper injectingProcess = ::OpenProcess(
			PROCESS_CREATE_THREAD |
			PROCESS_QUERY_INFORMATION |
			PROCESS_VM_OPERATION |
			PROCESS_VM_WRITE |
			PROCESS_VM_READ,
			FALSE,
			pid
		);

		if (!injectingProcess)
		{
			emit signal_OnAboutLog(QString("OpenProcess fails with: %1").arg(::GetLastError()));
			emit signal_OnAboutLog(QString(80, '-'));
			return;
		}

		if (checkImageFileState(pathToDll) == ImageFileState::X64 &&
			processRunningUnderWOW64(injectingProcess))
		{
			emit signal_OnAboutLog("Error: different bitness of DLLs and injecting process...");
			emit signal_OnAboutLog(QString(80, '-'));
			return;
		}

		emit signal_OnAboutLog("Process was opened...");

		std::wstring path = pathToDll.toStdWString();

		LPVOID ptrToString = 
			::VirtualAllocEx(injectingProcess, nullptr, (path.size() + 1) * sizeof(wchar_t), MEM_COMMIT, PAGE_READWRITE);

		if (!ptrToString)
		{
			emit signal_OnAboutLog(QString("VirtualAllocEx fails with: %1").arg(::GetLastError()));
			emit signal_OnAboutLog(QString(80, '-'));
			return;
		}

		emit signal_OnAboutLog("Memory for 'path to dll' string allocated to the specified process...");

		if (!::WriteProcessMemory(injectingProcess, ptrToString, path.c_str(), (path.size() + 1) * sizeof(wchar_t), nullptr))
		{
			emit signal_OnAboutLog(QString("WriteProcessMemory fails with: %1").arg(::GetLastError()));
			emit signal_OnAboutLog(QString(80, '-'));
			return;
		}

		emit signal_OnAboutLog(QString("'path to dll' string injected to the specified process by %1 address..."));

		PTHREAD_START_ROUTINE startRoutine = 
			reinterpret_cast<PTHREAD_START_ROUTINE>(::GetProcAddress(::GetModuleHandle(TEXT("kernel32.dll")), "LoadLibraryW"));

		emit signal_OnAboutLog("Injecting...");

		Common::Win32::HandleRAIIWrapper remoteThread = 
			::CreateRemoteThread(injectingProcess, nullptr, 0, startRoutine, ptrToString, 0, nullptr);

		if (!remoteThread)
		{
			emit signal_OnAboutLog(QString("CreateRemoteThread fails with: %1").arg(::GetLastError()));
			emit signal_OnAboutLog(QString(80, '-'));
			return;
		}

		::WaitForSingleObject(remoteThread, INFINITE);

		emit signal_OnAboutLog(QString("DLL was successful injected by %1 address!"));
		emit signal_OnAboutLog(QString(80, '-'));
	}

	ApplicationController::ImageFileState ApplicationController::checkImageFileState(QString const& imageFile) const
	{
		IMAGE_DOS_HEADER imageDOSHeader;
		IMAGE_FILE_HEADER imageFileHeader;
		std::uint32_t peSignature = 0;
		std::uint16_t bitOfImageFile = 0;

		std::ifstream imageFileStream(imageFile.toStdString(), std::ios_base::binary);

		if (!imageFileStream)
		{
			return ImageFileState::ErrorSpecifiedImageFileDoesNotExists;
		}

		imageFileStream.read(reinterpret_cast<char*>(&imageDOSHeader), sizeof(imageDOSHeader));

		// Check signature of each image file
		if (imageDOSHeader.e_magic != 0x5A4D)
		{
			return ImageFileState::ErrorInvalidDOSSignature;
		}

		// go to the first byte of NT header
		imageFileStream.seekg(static_cast<std::streampos>(imageDOSHeader.e_lfanew));
		imageFileStream.read(reinterpret_cast<char*>(&peSignature), sizeof(peSignature));

		if (peSignature != 0x00004550)
		{
			return ImageFileState::ErrorInvalidPESignature;
		}

		imageFileStream.read(reinterpret_cast<char*>(&imageFileHeader), sizeof(imageFileHeader));

		if (imageFileHeader.SizeOfOptionalHeader == 0)
		{
			return ImageFileState::ErrorInvalidOptionalHeaderSize;
		}
		
		if (imageFileHeader.Machine == IMAGE_FILE_MACHINE_I386)
		{
			return ImageFileState::X32;
		}

		if (imageFileHeader.Machine == IMAGE_FILE_MACHINE_AMD64)
		{
			return ImageFileState::X64;
		}

		return ImageFileState::ErrorUndefinedBitOfImageFile;
	}

	bool ApplicationController::processRunningUnderWOW64(HANDLE hProcess) const
	{
		BOOL bResult = FALSE;
		SYSTEM_INFO sysInfo;

		::IsWow64Process(hProcess, &bResult);
		::GetNativeSystemInfo(&sysInfo);

		assert(sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL ||
			sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64);

		if (sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL)
		{
			return false;
		}

		return bResult;
	}

}