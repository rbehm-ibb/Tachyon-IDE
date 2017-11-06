// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 10/19/2017 by behm
// ******************************************************

#ifndef HELPVIEW_H
#define HELPVIEW_H

class HelpModel;

class HelpView : public QWidget
{
	Q_OBJECT
public:
	HelpView(HelpModel *model, QWidget *parent = 0);
protected:
private:
	HelpModel *m_model;
	QTableView *m_tabView;
	QTextBrowser *m_text;
signals:
//	void showHelp(QString txt);
private slots:
	virtual void currentChanged(const QModelIndex &current, const QModelIndex &previous);
	void view();
};

#endif // HELPVIEW_H
