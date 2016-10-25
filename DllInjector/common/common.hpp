#pragma once
#include <cassert>
#include <QString>
#include <windows.h>

#ifndef NDEBUG
#define VERIFY(F) assert(F)
#else 
#define VERIFY(F) F
#endif

namespace Common
{
	inline void showSimpleNotification(QString const& title, QString const& message, QMessageBox::Icon icon = QMessageBox::NoIcon)
	{
		QMessageBox messageBox;
		messageBox.setWindowTitle(title);
		messageBox.setText(message);
		messageBox.setIcon(icon);
		messageBox.exec();
	}

	namespace Win32
	{
		class HandleRAIIWrapper
		{
		public:
			HandleRAIIWrapper(HANDLE handle)
				: m_handle(handle)
			{}

			HandleRAIIWrapper(HandleRAIIWrapper const&) = delete;

			HandleRAIIWrapper(HandleRAIIWrapper&& other)
				: m_handle(std::move(other.m_handle))
			{
				other.m_handle = nullptr;
			}

			~HandleRAIIWrapper()
			{
				closeHandle();
			}

			operator HANDLE()
			{
				return m_handle;
			}

			HandleRAIIWrapper& operator=(HandleRAIIWrapper const& other)
			{
				closeHandle();

				m_handle = other.m_handle;
			}

		private:
			void closeHandle() const
			{
				if (m_handle)
				{
					::CloseHandle(m_handle);
				}
			}

		private:
			HANDLE m_handle;
		};
	}
}