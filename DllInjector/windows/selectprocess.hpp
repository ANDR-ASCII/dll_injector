#pragma once
#include "ui_selectprocess.h"
#include "models/processsnapshotmodel.hpp"
#include <QWidget>
#include <windows.h>

namespace AppSpace
{
	class SelectProcess : public QWidget
	{
		Q_OBJECT

	public:
		SelectProcess(QWidget * parent = nullptr);

		QAbstractItemModel* model() const;

		DWORD selectedProcessID() const;
		QString const& selectedProcessName() const;

	Q_SIGNALS:
		void signal_SelectionChanged();

	private Q_SLOTS:
		void slot_SelectProcess();

	private:
		void init();

	private:
		Ui::SelectProcess ui;
		DWORD m_selectedPid;
		QString m_selectedProcessName;
		ProcessSnapshotModel* m_model;
	};
}
