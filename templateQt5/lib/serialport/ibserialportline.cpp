// ******************************************************
// * copyright (C) 2015 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 12/26/2015 by behm
// ******************************************************

#include "ibserialportline.h"

extern bool showLine;

IBSerialPortLine::IBSerialPortLine(QString device, QObject *parent)
	: IBSerialPort(device, parent)
	, m_rxTimer(new QTimer(this))
	, m_bol(QByteArray())
	, m_eol(QByteArray("\n"))
	, m_ignore(QByteArray("\r"))
	, m_rxTimeoutMsec(1000)
	, m_maxLineLength(1000)
	, m_maxLines(20)
	, m_hadBol(false)
{
	connect(this, &QSerialPort::readyRead, this, &IBSerialPortLine::readRxdDataSlot);
	m_rxTimer->setSingleShot(true);
	connect(m_rxTimer, &QTimer::timeout, this, &IBSerialPortLine::rxTimeoutSlot);
}

IBSerialPortLine::IBSerialPortLine(QString device, int defaultBaud, QObject *parent)
	: IBSerialPort(device, defaultBaud, parent)
	, m_rxTimer(new QTimer(this))
	, m_bol(QByteArray())
	, m_eol(QByteArray("\n"))
	, m_ignore(QByteArray("\r"))
	, m_rxTimeoutMsec(1000)
	, m_maxLineLength(1000)
	, m_maxLines(20)
	, m_hadBol(false)
{
	connect(this, &QSerialPort::readyRead, this, &IBSerialPortLine::readRxdDataSlot);
	m_rxTimer->setSingleShot(true);
	connect(m_rxTimer, &QTimer::timeout, this, &IBSerialPortLine::rxTimeoutSlot);
}

IBSerialPortLine::~IBSerialPortLine()
{
}


QByteArray IBSerialPortLine::readLine()
{
	if (canReadLine())
	{
		return m_lines.dequeue();
	}
	else
	{
		return QByteArray();
	}
}

void IBSerialPortLine::sendLine(const QByteArray &line)
{
	write(m_bol);
	write(line);
	write(m_eol);
}

void IBSerialPortLine::readRxdDataSlot()
{
	QByteArray rx = readAll();
	if (! rx.isEmpty())
	{
		m_rxdData += rx;
//		qDebug() << Q_FUNC_INFO << isBreakEnabled();
		if (showLine)
		{
			qDebug() << objectName() << m_rxdData.toHex();
		}
		for (;;)
		{
			if (! m_hadBol)
			{
				int istart = m_bol.isEmpty() ? 0 : m_rxdData.indexOf(m_bol);
				if (istart >= 0)
				{
					m_hadBol = true;
					m_rxdData = m_rxdData.remove(0, istart + m_bol.length());
				}
			}
			if (! m_hadBol)
			{
				break;	// no start seq yet
			}

			int iend = m_rxdData.indexOf(m_eol);
			if (iend < 0)
			{
				break;	// no end yet
			}
			// split at eol seq
			const QByteArray line = m_rxdData.left(iend).replace(m_ignore, QByteArray());	// between bol and eol
			m_rxdData = m_rxdData.remove(0, iend + m_eol.length());	// after eol seq
			m_hadBol = false;	// has been consumed
			if (line.length() > m_maxLineLength)
			{
				qWarning() << Q_FUNC_INFO << objectName() << "length OV" << line.length();
				emit rxOverflow();
			}
			else
			{
				if (m_lines.count() >= m_maxLines)
				{
					qWarning() << Q_FUNC_INFO << objectName() << "too many lines" << m_lines;
					emit rxOverflow();
				}
				else
				{
					m_lines.enqueue(line);
					if (showLine)
					{
						qDebug() << objectName() << line;
					}
				}
			}
		}
		if (m_rxdData.length() > m_maxLineLength)	// no bol or eol seq. Just noise or already too long
		{
			qWarning() << Q_FUNC_INFO << objectName() << "length OV" << m_rxdData.length();
			m_hadBol = false;
			m_rxdData.clear();
			emit rxOverflow();
		}
		if (m_hadBol && m_rxTimeoutMsec > 0)
		{
			m_rxTimer->start(m_rxTimeoutMsec);
		}
		else
		{
			m_rxTimer->stop();
		}
		if (canReadLine())
		{
			emit lineRxd();
		}
	}
}

void IBSerialPortLine::rxTimeoutSlot()
{
	if (m_hadBol)
	{
		qWarning() << Q_FUNC_INFO << objectName() << m_hadBol<< m_rxdData;
		m_hadBol = false;
		m_rxdData.clear();
		emit rxTimeout();
	}
}














