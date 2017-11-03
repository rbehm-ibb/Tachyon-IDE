// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 1/25/2017 by behm
// ******************************************************

#ifndef CONSOLE
#define CONSOLE

class Console : public QPlainTextEdit
{
	Q_OBJECT
public:
	explicit Console(QWidget *parent = 0);
	void closeIt() { m_close = true; }

signals:
	void sendSerial(QByteArray bytes);
	void beep();

public slots:
	void clear();
	void charRxd(char ch);
	void sendDate();
private:

	void sendPort(QString s);

	bool isEnabled;

	int  maxcol;
	int  maxrow;

	int  wrapMode;
	int  tabsize;

	bool m_close;	// may close

protected:
	void keyPressEvent(QKeyEvent *event);
	void resizeEvent(QResizeEvent *e);


	// QWidget interface
protected:
	virtual void closeEvent(QCloseEvent *ev);
};
#endif
