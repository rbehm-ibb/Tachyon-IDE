// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 10/19/2017 by behm
// ******************************************************

#include "helpview.h"
#include "helpmodel.h"

HelpView::HelpView(HelpModel *model, QWidget *parent)
	: QWidget(parent)
	, m_model(model)
	, m_tabView(new QTableView)
	, m_text(new QTextBrowser)
{
	QVBoxLayout *vlay = new QVBoxLayout(this);
	vlay->setMargin(0);
	vlay->setSpacing(0);
	vlay->addWidget(m_tabView);
	vlay->addWidget(m_text);
	m_tabView->setModel(model);
	m_tabView->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_tabView->setSelectionMode(QAbstractItemView::SingleSelection);
	QHeaderView *header = m_tabView->horizontalHeader();
	for (int i = 0; i < header->count(); ++i)
	{
		header->setSectionResizeMode(i, QHeaderView::ResizeToContents);
	}
	header->setStretchLastSection(true);
	connect(m_tabView->selectionModel(), &QItemSelectionModel::currentChanged, this, &HelpView::currentChanged);
	QAction *actView = new QAction(QIcon(":/icons/pics/eye.png"), "View", this);
	actView->setShortcut(QKeySequence("Ctrl+V"));
	connect(actView, &QAction::triggered, this, &HelpView::view);
	m_tabView->addAction(actView);
	m_tabView->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void HelpView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
	Q_UNUSED(previous);
	const HelpItem item = m_model->item(current);
	if (! item.file.isEmpty())
	{
		QString txt;
		if (! item.header.isEmpty())
		{
			txt += "<p>" + item.header + "</p>";
		}
		QString w = item.word;
		w.replace("<", "&lt;");
		w.replace(">", "&gt;");
		txt += "<b>" + w + "</b> <i>" + item.parms + "</i><p>" + item.description;
		txt += "<p><b>@" + item.file + "</b></p>";
		m_text->setText(txt);
	}
}

void HelpView::view()
{
	QModelIndex idx = m_tabView->currentIndex();
	if (idx.isValid())
	{
		QString hlp = idx.sibling(idx.row(), 0).data().toString();
//		qDebug() << Q_FUNC_INFO << idx << hlp;
		if (! hlp.isEmpty())
		{
			QDir dir(qApp->applicationDirPath() + "/help");
			QFile f(dir.absoluteFilePath(hlp + ".TXT"));
			if (! f.open(QIODevice::ReadOnly))
			{
				qWarning() << Q_FUNC_INFO << f.fileName() << f.errorString();
				return;
			}
			QString txt = f.readAll();
			m_text->setText(txt);
		}
	}
}
