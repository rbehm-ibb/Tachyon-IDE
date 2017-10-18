// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 1/25/2017 by behm
// ******************************************************

#include "wordsmodel.h"
#include "config.h"

WordsModel::WordsModel(QObject *parent)
	: QAbstractTableModel(parent)
	, m_header( { "Word", "Def", "Lineno" })
{

}

WordsModel::~WordsModel()
{
}

void WordsModel::save()
{
	Config::conf()->beginWriteArray("words", m_data.size());
	for (int i = 0; i < m_data.size(); ++i)
	{
		Config::conf()->setArrayIndex(i);
		Config::conf()->setValue("word", m_data[i].word);
		Config::conf()->setValue("def", m_data[i].def);
		Config::conf()->setValue("lineno", m_data[i].lineno);
	}
}

void WordsModel::sort()
{
	QMap<QString, WordItem> map;
	foreach (const WordItem &item, m_data)
	{
		map.insert(item.word, item);
	}
	beginResetModel();
	m_data.clear();
	foreach (const WordItem &item, map)
	{
		m_data.append(item);
	}
	endResetModel();
}

void WordsModel::analyse(QString source)
{
	const QStringList deftags( { ":", "pub", "pri", "pre", "CONSTANT", "==",
				  "TABLE", "BYTES", "WORDS:", "LONGS", "BYTE", "WORD", "LONG", "DOUBLE", "AVAR", "DS", "TIMER"  });
	const QStringList comtags( { "\\", "''", "---", "//", "\\\\\\",  "(", " {" } );
	beginResetModel();
	m_data.clear();
	const QRegExp sep("\\s");
	const QStringList lines = source.split('\n', QString::KeepEmptyParts);
	for (int lineno = 0; lineno < lines.size(); ++lineno)
	{
		const QStringList words = lines[lineno].split(sep, QString::SkipEmptyParts);
//		qDebug() << Q_FUNC_INFO << lineno << lines[lineno] << words;
		for (int i = 0; i < words.size(); ++i)
		{
			if (comtags.contains(words[i]))
				break;
			if (deftags.contains(words[i]))
			{
				WordItem wi;
				wi.def = words[i];
				if (i+1 < words.size())
					wi.word = words[i+1];
				wi.lineno = lineno;
				m_data.append(wi);
			}
		}
	}
	endResetModel();
}


void WordsModel::clear()
{
	beginResetModel();
	m_data.clear();
	endResetModel();
}


QModelIndex WordsModel::index(int row, int column, const QModelIndex &parent) const
{
	return QAbstractTableModel::index(row, column, parent);
}

int WordsModel::rowCount(const QModelIndex &) const
{
	return m_data.size();
}

int WordsModel::columnCount(const QModelIndex &) const
{
	return NCols;
}

bool WordsModel::hasChildren(const QModelIndex &) const
{
	return false;
}

QVariant WordsModel::data(const QModelIndex &index, int role) const
{
	if (index.row() >= 0 && index.row() < m_data.size())
	{
		switch (role)
		{
		case Qt::DisplayRole:
			switch (index.column())
			{
			case Word:
				return m_data[index.row()].word;
			case Def:
				return m_data[index.row()].def;
			case Lineno:
				return m_data[index.row()].lineno;
			}

			break;
		}
	}
	return QVariant();
}

QVariant WordsModel::headerData(int section, Qt::Orientation orientation, int role) const
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

Qt::ItemFlags WordsModel::flags(const QModelIndex &) const
{
	Qt::ItemFlags flags =  Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemNeverHasChildren;
	return flags;
}
