#pragma once

#include "../application/applicationcontroller.h"
#include "../models/processsnapshotmodel.h"
#include "selectprocess.h"
#include "ui_mainframe.h"
#include <memory>
#include <QtWidgets/QMainWindow>
#include <QString>

namespace Injector
{
class MainFrame : public QMainWindow
{
	Q_OBJECT

public:
	MainFrame(QWidget *parent = nullptr);

	void onAboutLogActions(QString const& str) const;

Q_SIGNALS:
	void signal_StartInjection(ApplicationController::ParametersPackForInject const& pack);

private Q_SLOTS:
	void slot_RunButtonClicked();
	void slot_SelectProcess();
	void slot_ProcessSelectionChanged();
	void slot_ShowFileDialog();

private:
	void init();

private:
	Ui::DllInjectorClass ui;
	SelectProcess* m_selectProcessWindow;
};

}