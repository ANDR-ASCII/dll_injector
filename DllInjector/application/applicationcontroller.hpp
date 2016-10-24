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
		void createRemoteThread(DWORD pid, QString const& pathToDll);

	Q_SIGNALS:
		void signal_OnError(QString);
		void signal_OnAboutLog(QString);

	private:
		bool m_seDebugPrivilege;
	};
}