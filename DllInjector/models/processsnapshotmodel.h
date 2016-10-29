#pragma once

#include "common/common.h"
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <QTimer>
#include <windows.h>
#include <tlhelp32.h>

namespace Injector
{

	class ProcessSnapshotModel : public QAbstractTableModel
	{
		Q_OBJECT

	private:
		struct ProcessInfo
		{
			std::wstring processName;
			DWORD processID;
			DWORD parentProcessID;
			DWORD countThreads;
			DWORD priorityClass;
		};

	public:
		enum SearchType
		{
			  ByPID
			, ByName
		};

		enum FieldType
		{
			  Name
			, PID
			, PriorityClass
		};

		ProcessSnapshotModel(QObject * parent = Q_NULLPTR);
		~ProcessSnapshotModel() = default;

		void update(DWORD = 0);

		QModelIndex search(QString const& str, SearchType type);
		bool existsItemOf(QModelIndex const& index) const;
		QVariant getValue(QModelIndex const& index) const;
		QVariant getValue(int row, int column) const;

		// QAbstractTableModel implementation
		virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
		virtual int rowCount(const QModelIndex &parent = QModelIndex{}) const override;
		virtual int columnCount(const QModelIndex &parent = QModelIndex{}) const override;
		virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

	Q_SIGNALS:
		void signal_OnError(QString str);

	private Q_SLOTS:
		void slot_TimedUpdater();

	private:
		void setHorizontalHeaderLabels(QStringList const& headerLabels);
		void setVerticalHeaderLabels(QStringList const& headerLabels);
		int headerLabelsSize(int role) const;

		void clear();

		QModelIndex searchByPID(int pid);
		QModelIndex searchByName(QString const& str);

		std::pair<bool, ProcessInfo> createItem(PPROCESSENTRY32W pEntry) const;
		QVariant retrievePriorityClass(DWORD priorityClass) const;

	private:
		std::vector<ProcessInfo> m_storage;
		QTimer* m_timer;
		QStringList m_HorizontalHeaderData;
		QStringList m_VecticalHeaderData;
	};

}