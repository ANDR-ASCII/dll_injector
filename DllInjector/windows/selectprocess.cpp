#include <stdafx.h>
#include "selectprocess.hpp"

namespace AppSpace
{
	SelectProcess::SelectProcess(QWidget * parent) : QWidget(parent)
	{
		ui.setupUi(this);
	}

	void SelectProcess::setModel(QAbstractTableModel* model)
	{
		ui.processView->setModel(model);
	}

	QAbstractItemModel* SelectProcess::model() const
	{
		return ui.processView->model();
	}

}