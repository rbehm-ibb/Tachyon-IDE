// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 1/24/2017 by behm
// ******************************************************

#ifndef PROP_H
#define PROP_H

class IBSerialPort;

class Prop : public QObject
{
	Q_OBJECT
public:
	explicit Prop(const QString name, uint baud, QObject *parent = 0);
	~Prop();

	IBSerialPort *port() const { return m_port; }
	QString device() const;
	QString portName() const;
	uint baud() const;

signals:
	void byteRxd(char c);
	void deviceChanged(const QString deviceBaud) const;

public slots:
	void setDevice(const QString device, uint baud) const;
	void send(const QByteArray ba);
	void upload(const QByteArray ba);
	void resetProp();
private slots:
	void readyRead();
	void timerTick();
protected:
private:
	IBSerialPort *m_port;
	bool m_lastWasSpace;
	QByteArray m_uploadData;
	int m_uploadIdx;
	QTimer *m_timer;
};

#endif // PROP_H
