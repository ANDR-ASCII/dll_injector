#ifndef DLLINJECTOR_H
#define DLLINJECTOR_H

#include "ui_dllinjector.h"
#include <QtWidgets/QMainWindow>
#include <QString>

namespace AppSpace
{
	class DllInjector : public QMainWindow
	{
		Q_OBJECT

	public:
		DllInjector(QWidget *parent = nullptr);

	private Q_SLOTS:
		void slot_RunButtonClicked();
		void slot_ShowErrorMessageBox(QString const& str);

	private:
		Ui::DllInjectorClass ui;
	};
}

#endif // DLLINJECTOR_H
