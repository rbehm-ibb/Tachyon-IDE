// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 1/25/2017 by behm
// ******************************************************

#ifndef FINDDIALOG_H
#define FINDDIALOG_H


class FindDialog : public QWidget
{
	Q_OBJECT
public:
	explicit FindDialog(QWidget *parent = 0);
	void focusIn() { findString->setFocus(); }
	void setFindString(const QString &s) { findString->setText(s); }
signals:
	void find(const QString exp, QTextDocument::FindFlags options);
public slots:
//	void findNext();
	void findSlot();
	void findBackSlot();
private slots:
protected:
private:
	QLineEdit *findString;
};

#endif // FINDDIALOG_H
