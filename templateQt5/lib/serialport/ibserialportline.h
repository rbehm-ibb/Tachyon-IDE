// ******************************************************
// * copyright (C) 2015 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 12/26/2015 by behm
// ******************************************************

#ifndef IBSERIALPORTLINE_H
#define IBSERIALPORTLINE_H

#include "ibserialport.h"

///
/// \brief The IBSerialPortLine class handles generalized line oriented transmission via serial ports.
///
/// Lines are defined as beginning with a BOL sequence and and with a EOL sequence.
/// Anything between these sequences is treated as a "line". In addition "ignore" sequnces are
/// stripped from the line.
/// Anything before BOL is ignored. If BOL is empty anything rxd is taken.
///
/// The default is BOL empty, EOL = \\n and IGNORE = \\r. This will receive normal lines which end in \\n or \\r\\n.
///
/// A rxd lines are signalled with lineRxd() and can be retreived with readLine().
/// The BOL, IGNORE and EOL sequences are already stripped.
/// Up to maxLines() rxd lines are stored in a queue.
/// If more are rxd before retrieving them, the signal rxOverflow() is raised and the lines will be discarded.
/// This happens also when when a line is longer than maxLineLength().
///
/// Bytes in line must be received with spacing < rxTimeout().
/// If a BOL has been rxd and no EOL that the signal rxTimeout() is raised.
///
/// An example of generalized lines is the NMEA protocol where a line starts with "$" end in \\r\\n.
///
/// For debugging the __external bool showline__ can be set.
/// The all rxd lines will we output via qDebug().
///
class IBSerialPortLine : public IBSerialPort
{
	Q_OBJECT
public:
	/// \brief Creates a IBSerialPortLine
	/// \param device The device to use (ttySx) with optional baudrate appended
	/// \param parent For QObject
	IBSerialPortLine(QString device, QObject *parent = 0);
	IBSerialPortLine(QString device, int defaultBaud, QObject *parent = 0);
	~IBSerialPortLine();
	bool ok() const { return isOpen(); }				///< valid and open?
	QByteArray readLine();						///< get a line if avail, empty if non avail
	bool canReadLine() const { return ! m_lines.isEmpty(); }	///< any lines available?

	void setBol(const QByteArray bol) { m_bol = bol; }		///< set the \param bol begin of line pattern.
	void setBol(const quint8 bol) { m_bol = QByteArray(1, bol); }		///< set the \param bol begin of line pattern.
	QByteArray bol() const { return m_bol; }			///< get the begin of line pattern.
	void setEol(const QByteArray eol) { m_eol = eol; }		///< set the \param eol end of line pattern
	void setEol(const quint8 eol) { m_eol = QByteArray(1, eol); }	///< set the \param eol end of line pattern.
	QByteArray eol() const { return m_eol; }			///< get the end of line pattern
	void setIgnore(const QByteArray ignore) { m_ignore = ignore; }	///< set the pattern to be ignored
	void setIgnore(const quint8 ignore) { m_ignore = QByteArray(1, ignore); } ///< set the pattern to be ignore
	void clrIgnore() { m_ignore = QByteArray(); }			///< reset ignore line pattern.
	QByteArray ignore() const { return m_ignore; }			///< get the ignore pattern
	void setMaxLineLength(int n) { m_maxLineLength = n; }		///< set the \param n max length of a line accepted
	int maxLineLength() const { return m_maxLineLength; }		///< get the max line length
	void setMaxLine(int n) { m_maxLines = n; }			///< set the \param n max number of lines stored internally
	int maxLines() const { return m_maxLines; }			///< get the max lines
	void setRxTimeout(int msec) { m_rxTimeoutMsec = msec; }		///< set the \param msec byte to byte timeout
	int rxTimeout() const { return m_rxTimeoutMsec; }		///< get the byte to byte timeout

signals:
	void lineRxd();		///< we have received a line
	void rxOverflow();	///< we had some overflow during reception (line too long, too many lines)
	void rxTimeout();	///< timeout inside a line
public slots:
	void sendLine(const QByteArray &line);	///< send a line with bol and eol patterns
private slots:
	void readRxdDataSlot();	///< internal, read rxd data fom SerialPort, called when data rxd.
	void rxTimeoutSlot();	///< handle byte to byte timeouts
protected:
	QTimer *m_rxTimer;	///<  handle byte to byte timeouts
	QByteArray m_bol;	///< begin of line pattern, default = empty
	QByteArray m_eol;	///< end of line pattern, default = \\n
	QByteArray m_ignore;	///< ignore pattern, default = \\r.
	int m_rxTimeoutMsec;	///< the byte to byte timeout in msec
	int m_maxLineLength;	///< max length of a line accepted, default = 1000
	int m_maxLines;		///< max number of line stored internally, default = 10
	bool m_hadBol;		///< did we receive a bol?
	QByteArray m_rxdData;	///< collects the next received line
	QQueue<QByteArray> m_lines;	///< collects already received lines.
};

#endif // IBSERIALPORTLINE_H
