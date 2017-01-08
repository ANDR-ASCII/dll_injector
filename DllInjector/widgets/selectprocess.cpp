#include "stdafx.h"
#include "selectprocess.h"
#include "common/common.h"
#include "models/processsnapshotmodel.h"

namespace Injector
{
	using namespace Common;

	SelectProcess::SelectProcess(QWidget * parent) 
		: QWidget(parent)
		, m_model(new ProcessSnapshotModel(this))
	{
		init();
	}

	void SelectProcess::init()
	{
		ui.setupUi(this);
		ui.processView->setModel(m_model);
		ui.processView->setAlternatingRowColors(true);
		ui.processView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

		VERIFY(connect(ui.selectButton, SIGNAL(clicked()), this, SLOT(slot_SelectProcess())));
	}

	QAbstractItemModel* SelectProcess::model() const
	{
		return ui.processView->model();
	}

	DWORD SelectProcess::selectedProcessID() const
	{
		return m_selectedPid;
	}

	QString const& SelectProcess::selectedProcessName() const
	{
		return m_selectedProcessName;
	}

	void SelectProcess::slot_SelectProcess()
	{
		QItemSelectionModel* selectionModel = ui.processView->selectionModel();

		if (selectionModel->hasSelection())
		{
			QModelIndexList modelIndexes = selectionModel->selectedIndexes();

			auto selectedOnlyOneRow = [&modelIndexes]
			{
				int row = modelIndexes.back().row();
				for (auto beg = modelIndexes.begin(), end = modelIndexes.end() - 1; beg != end; ++beg)
				{
					if (beg->row() != row)
					{
						return false;
					}
				}

				return true;
			};

			assert(selectedOnlyOneRow());

			assert(dynamic_cast<ProcessSnapshotModel*>(ui.processView->model()));

			ProcessSnapshotModel* model = static_cast<ProcessSnapshotModel*>(ui.processView->model());

			// here we can use any element
			// because all rows in all elements are equals
			int row = modelIndexes.back().row();

			m_selectedPid = static_cast<DWORD>(model->getValue(row, ProcessSnapshotModel::PID).toInt());
			m_selectedProcessName = model->getValue(row, ProcessSnapshotModel::Name).toString();

			this->hide();
			emit signal_SelectionChanged();
		}
		else
		{
			showSimpleNotification(
				"Notification", 
				"Please select the some process!", 
				QMessageBox::Information
			);
		}
	}

}