// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 10/23/2017 by behm
// ******************************************************

#include "interfaceconfigdialog.h"
#include "ui_interfaceconfigdialog.h"
#include "prop.h"

InterfaceConfigDialog::InterfaceConfigDialog(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::InterfaceConfigDialog)
{
	ui->setupUi(this);
	foreach (const QSerialPortInfo &pi, QSerialPortInfo::availablePorts())
	{
		QString pn = pi.portName();
		if (! pi.description().isEmpty())
		{
			pn += ": " +pi.description();
		}
		if (pi.hasVendorIdentifier())
		{
			pn += QString(" 0x%1:%2").arg(pi.vendorIdentifier(),4, 16,QChar('0')).arg(pi.productIdentifier(),4, 16,QChar('0'));
		}
		ui->portCombo->addItem(pn, pi.portName());
	}
	foreach (uint baud, QSerialPortInfo::standardBaudRates())
	{
		ui->baudCombo->addItem(QString::number(baud));
	}
}

InterfaceConfigDialog::~InterfaceConfigDialog()
{
	delete ui;
}

void InterfaceConfigDialog::setProp(Prop *prop)
{
	m_prop = prop;
	int idx = ui->portCombo->findData(prop->portName());
	ui->portCombo->setCurrentIndex(idx);
	ui->baudCombo->setCurrentText(QString::number(prop->baud()));
}


void InterfaceConfigDialog::accept()
{
	if (m_prop)
	{
		m_prop->setDevice(ui->portCombo->currentData().toString(), ui->baudCombo->currentText().toUInt());
	}
	close();
}
