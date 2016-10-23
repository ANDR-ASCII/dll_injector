#pragma once
#include <cassert>
#include <QString>

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
}