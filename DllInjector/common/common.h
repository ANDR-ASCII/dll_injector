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

class DisablerWidgetGuard
{
public:
	DisablerWidgetGuard(QWidget* widget)
		: m_widget(widget)
	{
		m_widget->setEnabled(false);
	}

	~DisablerWidgetGuard()
	{
		m_widget->setEnabled(true);
	}

private:
	QWidget* m_widget;
};

namespace Win32
{
	class Handle
	{
	public:
		Handle(HANDLE handle)
			: m_handle(handle)
		{}

		Handle(Handle const&) = delete;

		Handle(Handle&& other)
			: m_handle(std::move(other.m_handle))
		{
			other.m_handle = nullptr;
		}

		~Handle()
		{
			closeHandle();
		}

		operator HANDLE()
		{
			return m_handle;
		}

		Handle& operator=(Handle const& other)
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