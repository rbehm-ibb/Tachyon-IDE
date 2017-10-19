// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 1/25/2017 by behm
// ******************************************************

#include "wordsview.h"
#include "wordsmodel.h"

WordsView::WordsView(WordsModel *model, QWidget *parent)
	: QTableView(parent)
	, m_model(model)
{
	setModel(model);
	QHeaderView *header = horizontalHeader();
	for (int i = 0; i < header->count(); ++i)
	{
		header->setSectionResizeMode(i, QHeaderView::ResizeToContents);
	}
	header->setStretchLastSection(true);
	QAction *sort = new QAction("Sort", this);
	sort->setShortcut(QKeySequence("Alt+S"));
	addAction(sort);
	connect(sort, &QAction::triggered, m_model, &WordsModel::sort);
	setContextMenuPolicy(Qt::ActionsContextMenu);
}

void WordsView::currentChanged(const QModelIndex &current, const QModelIndex &)
{
	emit currentMoved(current);
}
