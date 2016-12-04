#pragma once

#include "ilogger.h"
#include <type_traits>
#include <utility>
#include <memory>
#include <cassert>
#include <cstddef>
#include <QString>
#include <windows.h>

namespace WinApiHelpers
{

class SharedHandle
{
public:
	SharedHandle(HANDLE handle)
		: m_counter(new std::size_t{ 1 })
		, m_handle(handle)
	{ }

	SharedHandle(SharedHandle const& other)
		: m_counter(other.m_counter)
		, m_handle(other.m_handle)
	{
		++*m_counter;
	}

	SharedHandle(SharedHandle&& other)
		: m_counter(other.m_counter)
		, m_handle(other.m_handle)
	{
		other.m_counter = nullptr;
		other.m_handle = nullptr;
	}

	~SharedHandle()
	{
		reset();
	}

	operator bool()
	{
		return m_handle != nullptr;
	}

	void reset()
	{
		if (m_counter && !(--*m_counter))
		{
			m_deleter(m_handle, m_counter);
		}
	}

	void reset(HANDLE handle)
	{
		reset();

		m_counter = new std::size_t{ 1 };
		m_handle = handle;
	}

	std::size_t useCount() const
	{
		return *m_counter;
	}

	bool unique() const
	{
		return useCount() == 1;
	}

	void swap(SharedHandle& other)
	{
		std::swap(m_counter, other.m_counter);
		std::swap(m_handle, other.m_handle);
	}

	PHANDLE get()
	{
		return &m_handle;
	}

	const HANDLE* get() const
	{
		return &m_handle;
	}

private:
	struct InternalHandleDeleter
	{
		void operator()(HANDLE& handle, std::size_t*& pCounter) const
		{
			if (handle)
			{
				::CloseHandle(handle);
				handle = nullptr;

				delete pCounter;
				pCounter = nullptr;
			}
		}
	};

private:
	std::size_t* m_counter;
	HANDLE m_handle;
	InternalHandleDeleter m_deleter;
};

// I supply create this object only in Process wrapper
// already initialized (as open handle)
// and all I need the wait unless it's executes
// and close it
class Thread
{
public:
	Thread(SharedHandle handle)
		: m_handle(handle)
	{}

	operator bool()
	{
		return m_handle;
	}

	void wait(DWORD ms)
	{
		if (m_handle)
		{
			::WaitForSingleObject(*m_handle.get(), ms);
		}
	}

private:
	SharedHandle m_handle;
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

	bool virtualFree(LPVOID address, SIZE_T size, DWORD freeType) const
	{
		bool result = ::VirtualFreeEx(m_handle, address, size, freeType) != 0;
		
		if (result)
		{
			log("Virtual memory successful was freed");
		}
		else
		{
			log("Virtual memory cannot be freed");
		}

		return result;
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
			return SharedHandle{ nullptr };
		}
		
		if (m_openedThisProcess)
		{
			SharedHandle handle{
				createThreadInternal(::CreateThread,
				securityAttributes,
				stackSize,
				startAddress,
				parameter,
				creationFlags,
				nullptr)
			};

			if (handle)
			{
				log("Thread successful created");
			}

			return handle;
		}
		
		SharedHandle handle{
			createThreadInternal(::CreateRemoteThread,
			m_handle,
			securityAttributes,
			stackSize,
			startAddress,
			parameter,
			creationFlags,
			nullptr)
		};

		if (handle)
		{
			log("Remote thread successful created");
		}

		return handle;
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
	HANDLE createThreadInternal(F* f, Args&&... args) const
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