// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 10/23/2017 by behm
// ******************************************************

#ifndef INTERFACECONFIGDIALOG_H
#define INTERFACECONFIGDIALOG_H

#include <QDialog>

namespace Ui {
class InterfaceConfigDialog;
}
class Prop;

class InterfaceConfigDialog : public QDialog
{
	Q_OBJECT

public:
	explicit InterfaceConfigDialog(QWidget *parent = 0);
	~InterfaceConfigDialog();
	void setProp(Prop * prop);
signals:
	void setDevice(const QString device, uint baud) const;
	// QDialog interface
public slots:
	virtual void accept();

private:
	Ui::InterfaceConfigDialog *ui;
	QPointer<Prop> m_prop;
};

#endif // INTERFACECONFIGDIALOG_H
