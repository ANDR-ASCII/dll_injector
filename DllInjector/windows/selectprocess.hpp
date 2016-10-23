#pragma once
#include "ui_selectprocess.h"
#include <QWidget>

namespace AppSpace
{
	class SelectProcess : public QWidget
	{
		Q_OBJECT

	public:
		SelectProcess(QWidget * parent = nullptr);

		void setModel(QAbstractTableModel* model);
		QAbstractItemModel* model() const;

	private:
		Ui::SelectProcess ui;
	};
}
