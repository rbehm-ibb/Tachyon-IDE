// ******************************************************
// * copyright (C) 2016 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 5/6/2016 by behm
// ******************************************************

#include "toolbarspacer.h"

ToolBarSpacer::ToolBarSpacer(int width)
	: QWidget(0)
{
	if (width > 0)
	{
		setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
		setFixedWidth(width);
	}
	else
	{
		setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
	}
}

void ToolBarSpacer::addSpacer(QToolBar *tb, int width)
{
	tb->addWidget(new ToolBarSpacer(width));
}

void ToolBarSpacer::addAbout(QToolBar *tb, QWidget *parent, const char aboutSlot[])
{
	ToolBarSpacer::addSpacer(tb);
	tb->addAction(QIcon(":/info"), tr("&About"), parent, aboutSlot);
}
