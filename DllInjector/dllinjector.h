#ifndef DLLINJECTOR_H
#define DLLINJECTOR_H

#include "application/applicationcontroller.hpp"
#include "models/processsnapshotmodel.hpp"
#include "windows/selectprocess.hpp"
#include "ui_dllinjector.h"
#include <memory>
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
		void slot_SelectProcess();

	private:
		void init();

	private:
		Ui::DllInjectorClass ui;
		std::unique_ptr<SelectProcess> m_selectProcessWindow;
		ProcessSnapshotModel* m_processesSnapshotModel;
		ApplicationController* m_appController;
	};
}

#endif // DLLINJECTOR_H
