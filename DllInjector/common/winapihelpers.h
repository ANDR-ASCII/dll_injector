#pragma once

#include "../application/ilogger.h"
#include <type_traits>
#include <cassert>
#include <QString>
#include <windows.h>

namespace WinApiHelpers
{

// I supply create this object only in Process wrapper
// already initialized (as open handle)
// and all I need the wait unless it's executes
// and close it
class Thread
{
public:
	Thread(HANDLE handle)
		: m_handle(handle)
	{}

	~Thread()
	{
		close();
	}

	operator bool()
	{
		return m_handle;
	}

	void wait(DWORD ms)
	{
		if (m_handle)
		{
			::WaitForSingleObject(m_handle, ms);
		}
	}

	void close()
	{
		if (m_handle)
		{
			m_handle = nullptr;
			::CloseHandle(m_handle);
		}
	}

private:
	HANDLE m_handle;
};

class Process
{
public:
	Process()
		: m_openFlag(false)
		, m_openedThisProcess(false)
	{}

	Process(Injector::ILogger* logger)
		: m_logger(logger)
		, m_openFlag(false)
		, m_openedThisProcess(false)
	{}

	Process(Process const&&) = delete;
	Process(Process&&) = delete;

	~Process()
	{
		close();
	}

	operator bool() const
	{
		return m_openFlag;
	}
	
	HANDLE get() const
	{
		return m_handle;
	}

	void setLogger(Injector::ILogger const* logger)
	{
		m_logger = logger;
	}

	bool open(DWORD pid, DWORD desiredAccess)
	{
		if (m_openFlag)
		{
			close();
		}

		m_handle = ::OpenProcess(desiredAccess, FALSE, pid);

		if (!m_handle)
		{
			log(QString("OpenProcess fails with: %1 code error").arg(::GetLastError()));
		}
		else
		{
			log("Process opened");
			m_openFlag = true;

			if (pid == ::GetCurrentProcessId())
			{
				m_openedThisProcess = true;
			}
		}

		return m_openFlag;
	}

	void close()
	{
		if (m_openFlag)
		{
			resetFlags();
			::CloseHandle(m_handle);
		}
	}

	LPVOID virtualAlloc(LPVOID address, SIZE_T sizeofBytes, DWORD allocationType, DWORD pageProtect) const
	{
		assert(m_openFlag);

		if (!m_openFlag)
		{
			return nullptr;
		}

		LPVOID allocAddress = ::VirtualAllocEx(m_handle, address, sizeofBytes, allocationType, pageProtect);

		if (allocAddress == nullptr)
		{
			log(QString("VirtualAllocEx fails with: %1 code error").arg(::GetLastError()));
			return nullptr;
		}

		log("Virtual allocation success");

		return allocAddress;
	}

	bool writeProcessMemory(LPVOID baseAddress, LPCVOID buffer, SIZE_T sizeofBytes) const
	{
		assert(m_openFlag);

		if (!m_openFlag)
		{
			return false;
		}

		SIZE_T bytesWritten = 0;
		BOOL bResult = ::WriteProcessMemory(m_handle, baseAddress, buffer, sizeofBytes, &bytesWritten);

		if (!bResult)
		{
			log(QString("WriteProcessMemory fails with %1 code error").arg(::GetLastError()));
			return false;
		}

		if (bytesWritten < sizeofBytes)
		{
			log("Not all information written to process");
		}

		return bResult;
	}

	Thread createThread(LPSECURITY_ATTRIBUTES securityAttributes, 
		SIZE_T stackSize, 
		LPTHREAD_START_ROUTINE startAddress, 
		LPVOID parameter, 
		DWORD creationFlags) const
	{
		assert(m_openFlag);
		
		if (!m_openFlag)
		{
			return Thread(nullptr);
		}
		
		if (m_openedThisProcess)
		{
			Thread t = createThreadInternal(::CreateThread, 
				securityAttributes, 
				stackSize, 
				startAddress, 
				parameter, 
				creationFlags, 
				nullptr);

			if (t)
			{
				log("Thread successful created");
			}

			return t;
		}
		
		Thread t = createThreadInternal(::CreateRemoteThread,
			m_handle, 
			securityAttributes, 
			stackSize, 
			startAddress, 
			parameter, 
			creationFlags, 
			nullptr);

		if (t)
		{
			log("Remote thread successful created");
		}

		return t;
	}
private:
	void log(QString const& log) const
	{
		if (m_logger)
		{
			(*m_logger)(log);
		}
	}

	void resetFlags()
	{
		m_openFlag = false;
		m_openedThisProcess = false;
	}

	template <typename F, typename... Args>
	Thread createThreadInternal(F* f, Args&&... args) const
	{
		assert((typeid(f) == typeid(&::CreateThread)) || typeid(f) == typeid(&::CreateRemoteThread));

		assert(reinterpret_cast<void*>(f) == reinterpret_cast<void*>(&::CreateThread) ||
			reinterpret_cast<void*>(f) == reinterpret_cast<void*>(&::CreateRemoteThread));

		return f(std::forward<Args>(args)...);
	}

private:
	HANDLE m_handle;

	Injector::ILogger const* m_logger;

	bool m_openFlag;

	bool m_openedThisProcess;
};

}