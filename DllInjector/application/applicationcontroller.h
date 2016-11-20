#pragma once

#include "../common/winapihelpers.h"
#include <memory>
#include <QObject>
#include <QString>

namespace Injector
{

class MainFrame;

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

	enum InjectionMethod
	{
		  CreateRemoteThreadMethod
		, SetWindowsHookExMethod
		, WindowsRegistryMethod
	};

	ApplicationController(int argc, char** argv, QObject* parent = nullptr);

	int exec();

	void enableSeDebugPrivilege();
	void disableSeDebugPrivilege();

	bool seDebugPrivilege() const;

	// first injection method
	void createRemoteThread(DWORD pid, QString const& pathToDll);

	// this function determines whether passed file pe32 or pe32+
	ImageFileState checkImageFileState(QString const& imageFile) const;

	bool isProcessX64(WinApiHelpers::Process const& process) const;
	bool isProcessX32(WinApiHelpers::Process const& process) const;

private:
	void setSeDebugPrivilege(bool flag);

private Q_SLOTS:
	void slot_OnAboutInject(InjectionMethod method, DWORD pid, QString const& pathToDll);

private:
	QApplication m_app;

	std::unique_ptr<MainFrame> m_mainFrame;

	bool m_seDebugPrivilege;
};

}