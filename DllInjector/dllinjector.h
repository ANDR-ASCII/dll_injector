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
		void slot_ShowFileDialog();

	private:
		void init();

	private:
		class Disabler
		{
		public:
			Disabler(QWidget* widget)
				: m_widget(widget)
			{
				m_widget->setEnabled(false);
			}

			~Disabler()
			{
				m_widget->setEnabled(true);
			}

		private:
			QWidget* m_widget;
		};

	private:
		Ui::DllInjectorClass ui;
		std::unique_ptr<SelectProcess> m_selectProcessWindow;
		ProcessSnapshotModel* m_processesSnapshotModel;
		ApplicationController* m_appController;
	};
}

#endif // DLLINJECTOR_H
