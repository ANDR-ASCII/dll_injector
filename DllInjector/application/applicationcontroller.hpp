#pragma once

#include <QObject>
#include <QString>

namespace AppSpace
{
	class ApplicationController : public QObject
	{
		Q_OBJECT

	public:
		enum ImageFileState
		{
			  ErrorInvalidDOSSignature
			, ErrorInvalidPESignature
			, ErrorInvalidOptionalHeaderSize
			, ErrorSpecifiedImageFileDoesNotExists
			, ErrorUndefinedBitOfImageFile
			, X32
			, X64
		};

		ApplicationController(QObject* parent = nullptr)
			: QObject(parent)
			, m_seDebugPrivilege(false)
		{}

		void setSeDebugPrivilege(bool flag);
		bool seDebugPrivilege() const;

		// first injection method
		void createRemoteThread(DWORD pid, QString const& pathToDll);

		// this function determines whether passed file pe32 or pe32+
		ImageFileState checkImageFileState(QString const& imageFile) const;

		bool processRunningUnderWOW64(HANDLE hProcess) const;

	Q_SIGNALS:
		void signal_OnError(QString);
		void signal_OnAboutLog(QString);

	private:
		bool m_seDebugPrivilege;
	};
}