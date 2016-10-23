#include "stdafx.h"
#include "processsnapshotmodel.hpp"
#include "common/stringfeatures.hpp"

namespace AppSpace
{

	ProcessSnapshotModel::ProcessSnapshotModel(QObject* parent)
		: QAbstractTableModel(parent)
		, m_timer(new QTimer(this))
	{
		QStringList headerLabels;

		headerLabels
			<< "Process name"
			<< "PID"
			<< "Priority";

		setHorizontalHeaderLabels(headerLabels);
		update();

		VERIFY(connect(m_timer, SIGNAL(timeout()), this, SLOT(slot_TimedUpdater())));

		m_timer->start(300);
	}

	QModelIndex ProcessSnapshotModel::search(QString const& str, SearchType type)
	{
		switch (type)
		{
		case ByPID:
			return searchByPID(str.toInt());

		case ByName:
			return searchByName(str);

		default:
			return QModelIndex{};
		}
	}

	int ProcessSnapshotModel::rowCount(const QModelIndex & parent) const
	{
		Q_UNUSED(parent);

		return static_cast<int>(m_storage.size());
	}

	int ProcessSnapshotModel::columnCount(const QModelIndex & parent) const
	{
		Q_UNUSED(parent);

		return headerLabelsSize(Qt::Horizontal);
	}

	QVariant ProcessSnapshotModel::data(const QModelIndex& index, int role) const
	{
		if (!index.isValid() || !existsItemOf(index))
		{
			return QVariant();
		}

		if (role == Qt::DisplayRole)
		{
			return getValue(index);
		}

		if (role == Qt::DecorationRole && index.column() == FieldType::Name)
		{
			static QIcon icon("icons/CPU-Intel-icon.png");
			return icon;
		}

		return QVariant();
	}

	void ProcessSnapshotModel::update(DWORD pid)
	{
		Q_UNUSED(pid);

		clear();

		HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		if (hSnapshot == INVALID_HANDLE_VALUE)
		{
			emit signal_OnError("Cannot create a processes snapshot");

			return;
		}

		PROCESSENTRY32W peEntry32;
		peEntry32.dwSize = sizeof(peEntry32);

		if (::Process32First(hSnapshot, &peEntry32) == FALSE)
		{
			emit signal_OnError("Cannot retrieve information about some process");
		}

		do
		{
			std::pair<bool, ProcessInfo> pair = createItem(&peEntry32);

			if (pair.first)
			{
				m_storage.push_back(pair.second);
			}

		} while (::Process32Next(hSnapshot, &peEntry32));

		::CloseHandle(hSnapshot);
	}

	void ProcessSnapshotModel::slot_TimedUpdater()
	{
		update();
	}

	void ProcessSnapshotModel::setHorizontalHeaderLabels(QStringList const& headerLabels)
	{
		m_HorizontalHeaderData = headerLabels;
		emit headerDataChanged(Qt::Horizontal, 0, headerLabels.size());
	}

	void ProcessSnapshotModel::setVerticalHeaderLabels(QStringList const& headerLabels)
	{
		m_VecticalHeaderData = headerLabels;
		emit headerDataChanged(Qt::Vertical, 0, headerLabels.size());
	}

	int ProcessSnapshotModel::headerLabelsSize(int role) const
	{
		assert(role == Qt::Vertical || role == Qt::Horizontal);

		if (role == Qt::Horizontal)
		{
			return m_HorizontalHeaderData.size();
		}

		return m_VecticalHeaderData.size();
	}

	void ProcessSnapshotModel::clear()
	{
		m_storage.clear();
	}

	QModelIndex ProcessSnapshotModel::searchByPID(int pid)
	{
		typedef std::vector<ProcessInfo>::size_type size_type;

		for (size_type i = 0, sz = m_storage.size(); i < sz; ++i)
		{
			if (m_storage[i].processID == static_cast<DWORD>(pid))
			{
				return QAbstractTableModel::createIndex(static_cast<int>(i), 1, this);
			}
		}

		return QModelIndex();
	}

	QModelIndex ProcessSnapshotModel::searchByName(QString const& str)
	{
		typedef std::vector<ProcessInfo>::size_type size_type;

		std::wstring comparableString = Common::StringFeatures::toLower(str.toStdWString());

		for (size_type i = 0, sz = m_storage.size(); i < sz; ++i)
		{
			if (Common::StringFeatures::toLower(m_storage[i].processName) == comparableString)
			{
				return QAbstractTableModel::createIndex(static_cast<int>(i), 0, this);
			}
		}

		return QModelIndex();
	}

	bool ProcessSnapshotModel::existsItemOf(QModelIndex const& index) const
	{
		if (rowCount() == 0 ||
			index.row() >= rowCount() ||
			index.column() >= headerLabelsSize(Qt::Horizontal))
		{
			return false;
		}

		return true;
	}

	QVariant ProcessSnapshotModel::getValue(QModelIndex const& index) const
	{
		switch (index.column())
		{
		case FieldType::Name:
			return Common::StringFeatures::wcharToQString(m_storage[index.row()].processName.c_str());

		case FieldType::PID:
			return static_cast<int>(m_storage[index.row()].processID);

		case FieldType::PriorityClass:
			return retrievePriorityClass(m_storage[index.row()].priorityClass);
		}

		return QVariant{};
	}

	QVariant ProcessSnapshotModel::getValue(int row, int column) const
	{
		return getValue(QAbstractItemModel::createIndex(row, column, nullptr));
	}

	QVariant ProcessSnapshotModel::headerData(int section, Qt::Orientation orientation, int role) const
	{
		if (orientation == Qt::Horizontal && section >= m_HorizontalHeaderData.size() ||
			orientation == Qt::Vertical && section >= m_VecticalHeaderData.size())
		{
			return QString{};
		}

		auto const& container = orientation == Qt::Vertical ? m_VecticalHeaderData : m_HorizontalHeaderData;

		if (role == Qt::DisplayRole)
		{
			return container[section];
		}

		return QVariant{};
	}

	std::pair<bool, ProcessSnapshotModel::ProcessInfo> ProcessSnapshotModel::createItem(PPROCESSENTRY32W pEntry) const
	{
		ProcessInfo item
		{
			  pEntry->szExeFile
			, pEntry->th32ProcessID
			, pEntry->th32ParentProcessID
			, pEntry->cntThreads
			, 0 // invalid value
		};

		// init priority class

		HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pEntry->th32ProcessID);

		if (hProcess != nullptr)
		{
			item.priorityClass = ::GetPriorityClass(hProcess);
		}
		else
		{
			return std::make_pair(false, item);
		}

		::CloseHandle(hProcess);

		return std::make_pair(true, item);
	}

	QVariant ProcessSnapshotModel::retrievePriorityClass(DWORD priorityClass) const
	{
		switch (priorityClass)
		{
		case REALTIME_PRIORITY_CLASS:
			return QString{ "Real time" };

		case HIGH_PRIORITY_CLASS:
			return QString{ "High" };

		case ABOVE_NORMAL_PRIORITY_CLASS:
			return QString{ "Above normal" };

		case NORMAL_PRIORITY_CLASS:
			return QString{ "Normal" };

		case BELOW_NORMAL_PRIORITY_CLASS:
			return QString{ "Below normal" };

		case IDLE_PRIORITY_CLASS:
			return QString{ "Low" };
		}

		return QString{ "Cannot retrieve" };
	}

}