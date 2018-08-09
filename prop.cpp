// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 1/24/2017 by behm
// ******************************************************

#include "prop.h"
#include "ibserialport.h"
#include "config.h"

Prop::Prop(const QString name, uint baud, QObject *parent)
	: QObject(parent)
	, m_port(new IBSerialPort(name, baud, this))
	, m_lastWasSpace(false)
	, m_timer(new QTimer(this))
{
	if (m_port->isOpen())
	{
		Config::setValue("hw/port", m_port->portName());
		Config::setValue("hw/baud", m_port->baudRate());
	}
	connect(m_port, &IBSerialPort::readyRead, this, &Prop::readyRead);
	connect(m_timer, &QTimer::timeout, this, &Prop::timerTick);
//	qDebug() << Q_FUNC_INFO << m_port->portName() << m_port->baudRate();
}

Prop::~Prop()
{
}

QString Prop::device() const
{
	return m_port->device();
}

QString Prop::portName() const
{
	return m_port->portName();
}

uint Prop::baud() const
{
	return m_port->baudRate();
}

void Prop::setDevice(const QString portName, uint baud) const
{
	if (m_port->isOpen())
	{
		m_port->close();
	}
	m_port->setPortName(portName);
	m_port->setBaudRate(baud);
	if (! m_port->open(QIODevice::ReadWrite))
	{
		qWarning() << Q_FUNC_INFO << m_port->portName() << m_port->errorString();
//		return;
	}
	if (m_port->isOpen())
	{
		Config::setValue("hw/port", m_port->portName());
		Config::setValue("hw/baud", m_port->baudRate());
	}
	m_port->setParity(QSerialPort::NoParity);
	m_port->setDataBits(QSerialPort::Data8);
	m_port->setFlowControl(QSerialPort::NoFlowControl);
	m_port->setObjectName(portName);
	emit deviceChanged(m_port->device());
}

void Prop::send(const QByteArray ba)
{
	m_port->write(ba);
//	qDebug() << Q_FUNC_INFO << ba.toHex();
}

void Prop::upload(const QByteArray ba)
{
	m_uploadData = ba;
	m_uploadIdx = 0;
	timerTick();
}

void Prop::resetProp()
{
	m_port->setDataTerminalReady(true);
	usleep(1000);
	m_port->setDataTerminalReady(false);
}

void Prop::readyRead()
{
	const QByteArray d = m_port->readAll();
//	qDebug() << Q_FUNC_INFO << d.toHex();
	foreach (const char &c, d)
	{
		if (c)
			emit byteRxd(c);
	}
}

void Prop::timerTick()
{
	if (m_uploadIdx >= m_uploadData.size())
	{
		m_timer->stop();
		return;
	}
	char c = m_uploadData[m_uploadIdx++];
	if (c == '\n') c = '\r';
	m_port->write(&c, 1);
//	emit byteRxd(c);
	switch (c)
	{
	case '\t':
	case ' ':
		if (m_lastWasSpace)
		{
			m_timer->start(0);
		}
		else
		{
			m_timer->start(1);
		}
		m_lastWasSpace = true;
		break;
	case '\r':
		m_timer->start(5);
		break;
//	case '\n':
//		timer->start(20);
//		break;
	default:
		m_timer->start(0);
	}
	bool rc = m_port->waitForBytesWritten(10);
	if (! rc)
		qDebug() << Q_FUNC_INFO;
}


