#pragma once
#include "ui_selectprocess.h"
#include <QWidget>
#include <windows.h>

namespace AppSpace
{
	class SelectProcess : public QWidget
	{
		Q_OBJECT

	public:
		SelectProcess(QWidget * parent = nullptr);

		void setModel(QAbstractTableModel* model);
		QAbstractItemModel* model() const;

		DWORD selectedProcessID() const;

	private Q_SLOTS:
		void slot_SelectProcess();

	private:
		void init();

	private:
		Ui::SelectProcess ui;
		DWORD m_selectedPid;
	};
}
