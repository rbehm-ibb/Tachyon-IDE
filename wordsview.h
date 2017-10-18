// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 1/25/2017 by behm
// ******************************************************

#ifndef WORDSVIEW_H
#define WORDSVIEW_H

class WordsModel;

class WordsView : public QTableView
{
	Q_OBJECT
public:
	explicit WordsView(WordsModel *model, QWidget *parent = 0);

signals:
	void currentMoved(const QModelIndex &current);
public slots:
	// QAbstractItemView interface
protected slots:
	virtual void currentChanged(const QModelIndex &current, const QModelIndex &previous);
private slots:
protected:
private:
	WordsModel *m_model;
};

#endif // WORDSVIEW_H
