#pragma once

#include <QObject>
#include <QString>

namespace AppSpace
{
	class ApplicationController : public QObject
	{
		Q_OBJECT

	public:
		ApplicationController(QObject* parent = nullptr)
			: QObject(parent)
			, m_seDebugPrivilege(false)
		{}

		void setSeDebugPrivilege(bool flag);
		bool seDebugPrivilege() const;

		// first injection method
		bool createRemoteThread(DWORD pid, QString const& dllName);

	Q_SIGNALS:
		void signal_OnError(QString);

	private:
		bool m_seDebugPrivilege;
	};
}