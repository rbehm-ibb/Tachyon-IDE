// ******************************************************
// * copyright (C) 2015 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 12/26/2015 by behm
// ******************************************************

#include "ibserialport.h"

IBSerialPort::IBSerialPort(QString device, QObject *parent)
	: QSerialPort(parent)
{
	init(device, 0);
}

IBSerialPort::IBSerialPort(QString device, int defaultBaud, QObject *parent)
	: QSerialPort(parent)
{
	init(device, defaultBaud);
}

IBSerialPort::IBSerialPort(quint16 vid, quint16 pid, QString serNr, int baud, QObject *parent)
	: QSerialPort(parent)
{
	QSerialPortInfo device;
	foreach (QSerialPortInfo spi, QSerialPortInfo::availablePorts())
	{
//		qDebug() << Q_FUNC_INFO << hex << spi.portName() << spi.vendorIdentifier() << spi.productIdentifier() << spi.serialNumber();
		if (spi.vendorIdentifier() == vid && spi.productIdentifier() == pid)
		{
			if (! serNr.isEmpty() && (spi.serialNumber() != serNr))
			{
				continue;
			}
			device = spi;
			setObjectName(device.description());
			setPort(device);
			if (! open(QIODevice::ReadWrite))
			{
				qWarning() << Q_FUNC_INFO << portName() << errorString();
				return;
			}
			setBaudRate(baud);
			setParity(QSerialPort::NoParity);
			setDataBits(QSerialPort::Data8);
			setFlowControl(QSerialPort::NoFlowControl);
			return;
		}
	}
	if (! isOpen())
	{
		qWarning() << Q_FUNC_INFO << Qt::hex << "VID" << vid << "PID" << pid << "Ser#" << serNr << "not found" << Qt::dec;

	}
}

QString IBSerialPort::device() const
{
	return QString("%1:%2").arg(portName()).arg(baudRate());
}

void IBSerialPort::portError(QSerialPort::SerialPortError err)
{
	switch(err)
	{
	case QSerialPort::ResourceError:
		emit lostPortError();
		break;
	default:
		qWarning() << Q_FUNC_INFO << err << errorString();
	}
}

void IBSerialPort::init(QString dev, int defaultBaud)
{
	int baud = defaultBaud == 0 ? 9600 :defaultBaud;
	if (dev.contains(':'))
	{
		bool ok;
		QString sbaud = dev.section(':', 1);
		baud = sbaud.toUInt(&ok);
		if (! ok)
		{
			qWarning() << Q_FUNC_INFO << "Bad baudrate given" << dev << "using default";
		}
		dev = dev.section(':', 0, 0);
	}
	setPortName(dev);
	if (! open(QIODevice::ReadWrite))
	{
		qWarning() << Q_FUNC_INFO << portName() << errorString();
		return;
	}
	setBaudRate(baud);
	setParity(QSerialPort::NoParity);
	setDataBits(QSerialPort::Data8);
	setFlowControl(QSerialPort::NoFlowControl);
	setObjectName(device());
	connect(this, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error), this, &IBSerialPort::portError);
}
