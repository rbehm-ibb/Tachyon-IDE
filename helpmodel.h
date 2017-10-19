// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 10/19/2017 by behm
// ******************************************************

#ifndef HELPMODEL_H
#define HELPMODEL_H
///
/// \brief The HelpItem struct
/// holds the info extracted from the help files for each word.
struct HelpItem
{
	QString file;		///< where it was found
	QString word;		///< the FORTH word
	QString parms;		///< the parameters as found in the help file in FORTH standard notation ( stack-in -- stack out )
	QString description;	///< the description found in the help file
	QString header;	///< the description found in the help file
};

class HelpModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	enum Cols { File, Word, Params, Description, NCols };
	Q_ENUM(Cols)
	explicit HelpModel(QObject *parent = 0);
	~HelpModel();
	void load();	///< laod the help info from all files
	const HelpItem item(const QModelIndex &idx);

	// the standard model interface
	virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
	virtual int rowCount(const QModelIndex &) const { return m_data.count(); }
	virtual int columnCount(const QModelIndex &) const { return NCols; }
	virtual bool hasChildren(const QModelIndex &) const { return false; }
	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	virtual Qt::ItemFlags flags(const QModelIndex &) const;
	void clear();	///< clear the model
signals:

public slots:
private slots:
protected:
private:
	QVector<HelpItem> m_data;	///< the model data
	const QStringList m_header;	///< the strings for the header
};

#endif // HELPMODEL_H
