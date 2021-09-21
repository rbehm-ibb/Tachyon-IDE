// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 1/25/2017 by behm
// ******************************************************

#include "finddialog.h"

FindDialog::FindDialog(QWidget *parent)
	: QWidget(parent)
{
	QFormLayout *lay = new QFormLayout(this);
	lay->addRow("Text", findString = new QLineEdit);
	findString->setClearButtonEnabled(true);
	QPushButton *bf = new QPushButton("Find");
	QPushButton *br = new QPushButton("Find Rev");
	bf->setIcon(QIcon(":/icons/pics/find-next.png"));
	br->setIcon(QIcon(":/icons/pics/find-previous.png"));
	QHBoxLayout *hl = new QHBoxLayout;
	lay->addRow(hl);
	hl->addWidget(bf);
	hl->addWidget(br);
	connect(bf, &QPushButton::clicked, this, &FindDialog::findSlot);
	connect(br, &QPushButton::clicked, this, &FindDialog::findBackSlot);
	connect(findString, &QLineEdit::returnPressed, this, &FindDialog::findSlot);
}

void FindDialog::findSlot()
{
	const QString text = findString->text();
	QTextDocument::FindFlags options = QTextDocument::FindFlags();
	emit find(text, options);
}

void FindDialog::findBackSlot()
{
	const QString text = findString->text();
	QTextDocument::FindFlags options = QTextDocument::FindBackward;
	emit find(text, options);
}


