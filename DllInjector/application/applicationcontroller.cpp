#include "stdafx.h"
#include "../widgets/mainframe.h"
#include "../common/common.h"
#include "../common/winapihelpers.h"
#include "applicationcontroller.h"
#include "injectorlogger.h"
#include <fstream>
#include <cstdint>
#include <windows.h>
#include <winbase.h>

namespace Injector
{

ApplicationController::ApplicationController(int argc, char** argv, QObject* parent)
	: QObject(parent)
	, m_app(argc, argv)
	, m_mainFrame(new MainFrame)
	, m_seDebugPrivilege(false)
{
	enableSeDebugNamePrivilege();

	if (!seDebugPrivilege())
	{
		Common::showSimpleNotification("Notification", 
			"Cannot enable SE DEBUG PRIVILEGE, "
			"try to restart program as administrator");
	}

	VERIFY(connect(m_mainFrame.get(), &MainFrame::signal_StartInjection, this, &ApplicationController::slot_OnAboutInject));

	m_mainFrame->show();
}

int ApplicationController::exec()
{
	return m_app.exec();
}

void ApplicationController::enableSeDebugNamePrivilege()
{
	setSeDebugPrivilege(true);
}

void ApplicationController::disableSeDebugNamePrivilege()
{
	setSeDebugPrivilege(false);
}

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
		::CloseHandle(hThisProcessToken);
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
	InjectorLogger<MainFrame> logger(m_mainFrame.get(), &MainFrame::onAboutLogActions);
	logger(QString(65, '*'));

	WinApiHelpers::Process process(&logger);

	process.open(pid,
		PROCESS_CREATE_THREAD |
		PROCESS_QUERY_INFORMATION |
		PROCESS_VM_OPERATION |
		PROCESS_VM_WRITE |
		PROCESS_VM_READ
	);

	if (!process)
	{
		return;
	}

	if ((checkImageFileState(pathToDll) == ImageFileState::X64 && isProcessX32(process)) ||
		(checkImageFileState(pathToDll) == ImageFileState::X32 && isProcessX64(process)))
	{
		logger("<font color='red'>Error: different bits of DLLs and injecting process...</font>");
		return;
	}

	const std::wstring path = pathToDll.toStdWString();
	const std::size_t sizeOfBytes = (path.size() + 1) * sizeof(wchar_t);

	LPVOID ptrToString = process.virtualAlloc(nullptr, sizeOfBytes, MEM_COMMIT, PAGE_READWRITE);

	if (!ptrToString)
	{
		return;
	}

	if (!process.writeProcessMemory(ptrToString, path.c_str(), sizeOfBytes))
	{
		return;
	}

	PTHREAD_START_ROUTINE startRoutine = 
		reinterpret_cast<PTHREAD_START_ROUTINE>(::GetProcAddress(::GetModuleHandleW(L"kernel32.dll"), "LoadLibraryW"));

	WinApiHelpers::Thread thread = process.createThread(nullptr, 0, startRoutine, ptrToString, 0);

	if (!thread)
	{
		return;
	}

	thread.wait(INFINITE);
	process.virtualFree(ptrToString, 0, MEM_RELEASE);

	logger(QString(65, '*'));
}

ApplicationController::ImageFileState ApplicationController::checkImageFileState(QString const& imageFile) const
{
	IMAGE_DOS_HEADER imageDOSHeader;
	IMAGE_FILE_HEADER imageFileHeader;
	std::uint32_t peSignature = 0;

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

bool ApplicationController::isProcessX64(WinApiHelpers::Process const& process) const
{
	BOOL bResult = FALSE;
	SYSTEM_INFO sysInfo;

	::IsWow64Process(process.get(), &bResult);
	::GetNativeSystemInfo(&sysInfo);

	if (bResult == false && 
		sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
	{
		return true;
	}

	return false;
}

bool ApplicationController::isProcessX32(WinApiHelpers::Process const& process) const
{
	BOOL bResult = FALSE;
	SYSTEM_INFO sysInfo;

	::IsWow64Process(process.get(), &bResult);
	::GetNativeSystemInfo(&sysInfo);

	return bResult;
}

void ApplicationController::slot_OnAboutInject(InjectionMethod method, DWORD pid, QString const& pathToDll)
{
	switch (method)
	{
	case ApplicationController::InjectionMethod::CreateRemoteThreadMethod:
		createRemoteThread(pid, pathToDll);
		return;

	default:
		Common::showSimpleNotification(
			"Error",
			"Unfortunately this method while not implemented",
			QMessageBox::Critical
		);
	}
}

}