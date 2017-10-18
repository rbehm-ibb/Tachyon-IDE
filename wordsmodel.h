// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 1/25/2017 by behm
// ******************************************************

#ifndef WORDSMODEL_H
#define WORDSMODEL_H

struct WordItem
{
	QString word;
	QString def;
	int lineno;
};

class WordsModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	enum Cols { Word, Def, Lineno, NCols };
	Q_ENUM(Cols)
	WordsModel(QObject *parent);
	~WordsModel();
	void save();
	void sort();
	void analyse(QString source);
	const QVector<WordItem> &words() const { return m_data; }
	// QAbstractItemModel interface
public:
	virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
	virtual int rowCount(const QModelIndex &) const;
	virtual int columnCount(const QModelIndex &) const;
	virtual bool hasChildren(const QModelIndex &) const;
	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	virtual Qt::ItemFlags flags(const QModelIndex &) const;
	void clear();
protected:
private:
	QVector<WordItem> m_data;
	const QStringList m_header;
};

#endif // WORDSMODEL_H
