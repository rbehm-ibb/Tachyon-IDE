// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 10/19/2017 by behm
// ******************************************************

#include "helpmodel.h"

HelpModel::HelpModel(QObject *parent)
	: QAbstractTableModel(parent)
	, m_header( { "File", "Word", "Params", "Description"})
{

}

HelpModel::~HelpModel()
{
}

void HelpModel::load()
{
	QDir dir(qApp->applicationDirPath() + "/help", "*.TXT");
	beginResetModel();
	m_data.clear();
	foreach (const QFileInfo &fi, dir.entryInfoList())
	{
		QFile f(fi.absoluteFilePath());
		if (! f.open(QIODevice::ReadOnly))
		{
			qWarning() << Q_FUNC_INFO << f.fileName() << f.errorString();
			continue;
		}
//		qDebug() << Q_FUNC_INFO << f.fileName();
		// parse the file, look for lines of the form "word ( xxx -- xx) text"
		QTextStream s(&f);
		QString header;
		bool hadAny =false;
		while (! s.atEnd())
		{
			QString line = s.readLine();
			if (line.isEmpty())
			{
				continue;
			}
			if (line.startsWith("---"))
			{
				header = line;
				continue;
			}
			if (line.contains("Examples:"))
			{
				break;
			}
			if (line.contains("("))	// very simple detection, could use RegEx here
			{
//				qDebug() << Q_FUNC_INFO << fi.baseName() << line;
				HelpItem item;
				item.file = fi.baseName();
				line = line.trimmed();
				item.word = line.section(' ', 0, 0).trimmed();
				line = line.section(' ', 1);
				item.parms = line.section(')', 0, 0).trimmed() + " )";
				item.description = line.section(')', 1).trimmed();
//				item.description = item.word + " " + item.parms + " " + line.section(')', 1).trimmed();
				if (! header.isEmpty())
				{
					item.header = header;
//					item.description = header + "\n" + item.description;
				}
//				item.description = "@ " + item.file + "\n" + item.description;
//				qDebug() << item.file << item.word << item.parms << item.description;
				m_data.append(item);
				hadAny = true;
			}
		}
		if (! hadAny)
		{
			HelpItem item;
			item.file = fi.baseName();
			m_data.append(item);
		}
	}
	endResetModel();
}

const HelpItem HelpModel::item(const QModelIndex &idx)
{
	if (idx.isValid() && idx.row() < rowCount(QModelIndex()))
	{
		return m_data[idx.row()];
	}
	return HelpItem();
}

void HelpModel::clear()
{
	beginResetModel();
	m_data.clear();
	endResetModel();
}


QModelIndex HelpModel::index(int row, int column, const QModelIndex &parent) const
{
	return QAbstractTableModel::index(row, column, parent);
}



QVariant HelpModel::data(const QModelIndex &index, int role) const
{
	if (index.row() >= 0 && index.row() < m_data.size())
	{
		switch (role)
		{
		case Qt::DisplayRole:
			switch (index.column())
			{
			case File:
				return m_data[index.row()].file;
			case Word:
				return m_data[index.row()].word;
			case Params:
				return m_data[index.row()].parms;
			case Description:
				return m_data[index.row()].description;
			}

			break;
		}
	}
	return QVariant();
}

QVariant HelpModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	Q_UNUSED(role)
	if (orientation == Qt::Horizontal)
	{
		if (section >= 0 && section < NCols)
		{
			return m_header[section];
		}
		return QVariant();
	}
	return section;
}

Qt::ItemFlags HelpModel::flags(const QModelIndex &) const
{
	Qt::ItemFlags flags =  Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemNeverHasChildren;
	return flags;
}
