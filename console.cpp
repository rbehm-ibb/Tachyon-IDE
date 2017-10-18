#include "console.h"
#include "config.h"

// It's Spinning Windows world.
#define SPIN_RETURN     0xA
#define SPIN_NEWLINE    0xD

Console::Console(QWidget *parent)
	: QPlainTextEdit(parent)
	, m_close(false)
{
	setWindowTitle(QCoreApplication::applicationName() + ": Terminal");
	setWindowIcon(QIcon(":/icons/pics/project-terminal.png"));
	QRect r = Config::value("ui_pos/console", QRect(QPoint(0, 0), QSize(640, 480))).toRect();
	move(r.topLeft());
	resize(r.size()); // initial size

	maxrow = 1000;
	isEnabled = true;
	pcmd = Console::PCMD_NONE;
	pcmdx = 0;
	pcmdy = 0;
	maxcol = 80;
	wrapMode = 0;
	tabsize = 8;
	hexmode = false;
	hexbytes = 0;
	maxhex = 16;
	enableEchoOn = false;
	enableNewLine = true;
	enableBackspace = true;

	enableClearScreen = true;
	enableHomeCursor = true;
	enablePosXYCursor = true;
	enableMoveCursorLeft = true;
	enableMoveCursorRight = true;
	enableMoveCursorUp = true;
	enableMoveCursorDown = true;
	enableBeepSpeaker = true;
	enableBackspace = true;
	enableTab = true;
	enableCReturn = true;
	enableClearToEOL = true;
	enableClearLinesBelow = true;
	enableNewLine = true;
	enablePosCursorX = true;
	enablePosCursorY = true;
	enableClearScreen16 = true;
	enableEchoOn = false;
	enableEnterIsNL = true;

	enableSwapNLCR = true;

	enableANSI = true;

	for(int n = 0; n < maxhex; n++)
	{
		hexbyte[n] = 0;
	}
	sbuff = NULL;

	newline = '\n';
	creturn = '\r';
	QAction *clear = new QAction(QIcon(":/remove"), "Clear", this);
	clear->setShortcut(QKeySequence("Alt+C"));
	connect(clear, &QAction::triggered, this, &Console::clear);
	addAction(clear);
	QAction *date = new QAction("Date", this);
	date->setShortcut(QKeySequence("Alt+D"));
	connect(date, &QAction::triggered, this, &Console::sendDate);
	addAction(date);
}

void Console::setPortEnable(bool value)
{
	utfparse = false;
	utfbytes = 0;
	utf8 = 0;
	hexbytes = 0;
	for(int n = 0; n < maxhex; n++)
	{
		hexbyte[n] = 0;
	}
	isEnabled = value;
}

bool Console::enabled()
{
	return isEnabled;
}

void Console::clear()
{
	utfparse = false;
	utfbytes = 0;
	utf8 = 0;
	hexbytes = 0;
	for(int n = 0; n < maxhex; n++)
	{
		hexbyte[n] = 0;
	}
	setPlainText("");
}

#if 0
void Console::eventKey(QKeyEvent *event)
{
	int key = event->key();

//	qDebug() << Q_FUNC_INFO << key;
	switch(key)
	{
	case Qt::Key_Enter:
	case Qt::Key_Return:
		if(enableEnterIsNL)
//			key = SPIN_NEWLINE;
//		else
//			key = SPIN_RETURN;
		case Qt::Key_Alt:
	case Qt::Key_Control:
	case Qt::Key_Shift:
		return;
	default:
		if(QApplication::keyboardModifiers() & Qt::CTRL)
		{
			key &= ~0xe0;
		}
		else
		{
			QString s = event->text();
			if(s.length() == 1)
			{
				QChar c = event->text().at(0);
				key = (int)c.toLatin1();
			}
			else
			{
				sendPort(s);
				return;
			}
		}
		break;
	}
}
#endif

void Console::keyPressEvent(QKeyEvent *event)
{
//	qDebug() << Q_FUNC_INFO << event;

	if(event->matches((QKeySequence::Paste)))
	{
		QClipboard *clip = QApplication::clipboard();
		QString s = clip->text();
		s = s.replace("\n", "\r");
		sendPort(s);
	}
	else
	{
		if(enableEchoOn)
		{
			insertPlainText(event->text());
		}
		int key = event->key();
//		qDebug() << Q_FUNC_INFO << 1 << key << event->text();

		switch(key)
		{
		case Qt::Key_Enter:
		case Qt::Key_Return:
			key = '\r';
			break;
		case Qt::Key_Alt:
		case Qt::Key_Control:
		case Qt::Key_Shift:
			return;
		default:
			if(QApplication::keyboardModifiers() & Qt::CTRL)
			{
				key &= ~0xe0;
			}
			else
			{
				QString s = event->text();
				if(s.length() == 1)
				{
					QChar c = event->text().at(0);
					key = (int)c.toLatin1();
//					qDebug() << Q_FUNC_INFO << 2 << key << c << event->text();
				}
				else
				{
					sendPort(s);
					return;
				}
			}
			break;
		}
		QByteArray barry;
		barry.append((char)key);
//		qDebug() << Q_FUNC_INFO << 3 << key << barry.toHex();
		emit sendSerial(barry);
	}
}

void Console::sendPort(QString s)
{
	QByteArray ba;
	foreach(QChar c, s.toUtf8())
	{
		ba.append(c);
		emit sendSerial(ba);
		ba.clear();
		thread()->yieldCurrentThread();
	}
}

void Console::resizeEvent(QResizeEvent *e)
{
	QFontMetrics fm(font());
	maxcol = width() / 8;
	if(fm.width("X") > 0)
	{
		maxcol = width() / fm.width("X") - 3;
	}

	//qDebug() << maxcol << width() << fm.width("X");
	QPlainTextEdit::resizeEvent(e);
}

void Console::setEnableClearScreen(bool value)
{
	enableClearScreen = value;
}
void Console::setEnableHomeCursor(bool value)
{
	enableHomeCursor = value;
}
void Console::setEnablePosXYCursor(bool value)
{
	enablePosXYCursor = value;
}
void Console::setEnableMoveCursorLeft(bool value)
{
	enableMoveCursorLeft = value;
}
void Console::setEnableMoveCursorRight(bool value)
{
	enableMoveCursorRight = value;
}
void Console::setEnableMoveCursorUp(bool value)
{
	enableMoveCursorUp = value;
}
void Console::setEnableMoveCursorDown(bool value)
{
	enableMoveCursorDown = value;
}
void Console::setEnableBeepSpeaker(bool value)
{
	enableBeepSpeaker = value;
}
void Console::setEnableBackspace(bool value)
{
	enableBackspace = value;
}
void Console::setEnableTab(bool value)
{
	enableTab = value;
}
void Console::setEnableCReturn(bool value)
{
	enableCReturn = value;
}
void Console::setEnableClearToEOL(bool value)
{
	enableClearToEOL = value;
}
void Console::setEnableClearLinesBelow(bool value)
{
	enableClearLinesBelow = value;
}
void Console::setEnableNewLine(bool value)
{
	enableNewLine = value;
}
void Console::setEnablePosCursorX(bool value)
{
	enablePosCursorX = value;
}
void Console::setEnablePosCursorY(bool value)
{
	enablePosCursorY = value;
}
void Console::setEnableClearScreen16(bool value)
{
	enableClearScreen16 = value;
}
void Console::setEnableEchoOn(bool value)
{
	enableEchoOn = value;
}
void Console::setEnableEnterIsNL(bool value)
{
	enableEnterIsNL = value;
}


void Console::setEnableSwapNLCR(bool value)
{
	enableSwapNLCR = value;
	if(enableSwapNLCR)
	{
		newline = 10;
		creturn = 13;
	}
	else
	{
		newline = 13;
		creturn = 10;
	}
}

bool Console::getHexMode()
{
	return hexmode;
}

int Console::getEnter()
{
	if(enableEnterIsNL)
		return newline;
	return creturn;
}

void Console::setWrapMode(int mode)
{
	wrapMode = mode;
	if(mode == 0)
	{
		setWordWrapMode(QTextOption::WordWrap);
	}
	else
	{
		setWordWrapMode(QTextOption::WrapAnywhere);
	}
}

void Console::setTabSize(int size)
{
	tabsize = size;
}

void Console::setHexMode(bool enable)
{
	hexmode = enable;
}

void Console::setHexDump(bool enable)
{
	hexdump = enable;
}

void Console::setMaxRows(int rows)
{
	maxrow = rows;
}

void Console::dumphex(int ch)
{
	unsigned char c = ch;
	QTextCursor cur = textCursor();
	// always start at the end just in case someone clicked the window
	moveCursor(QTextCursor::End);

	if(hexdump != true)
	{
		if(wrapMode > 0)
		{
			if(cur.block().length() > wrapMode)
				cur.insertBlock();
		}
		else if(cur.block().length() > maxcol - 2)
		{
			cur.insertBlock();
		}

		cur.insertText(QString(" %1").arg(c, 2, 16, QChar('0')));
	}
	else
	{
		int byte = hexbytes % maxhex;
		if(byte == 0)
		{
			cur.insertText(QString("  "));
			for(int n = 0; n < maxhex; n++)
			{
				if(isprint(hexbyte[n]))
					cur.insertText(QString(hexbyte[n]));
				else
					cur.insertText(QString("."));
			}
			cur.insertBlock();
		}
		hexbyte[byte] = ch;
		hexbytes++;

		cur.insertText(QString(" %1").arg(c, 2, 16, QChar('0')));
	}
}

void Console::sendDate()
{
	QString dt(" #%1 DATE! #%2 TIME!\r");
	QDateTime cdt = QDateTime::currentDateTime();
	dt = dt.arg(cdt.toString("yyMMdd")).arg(cdt.toString("HHmmss"));
	qDebug() << dt;
	sendPort(dt);
}

void Console::update(char ch)
{
//	qDebug() << Q_FUNC_INFO << hex << int(ch) << dec;
	switch (ch)
	{
	case 0x12:
		sendDate();
		return;
	case 0x0c:
		setPlainText("");
		return;
	}
	QString text = "";
	QTextCursor cur = textCursor();

	setMaximumBlockCount(maxrow);
	setWordWrapMode(QTextOption::WrapAnywhere);

	if(wrapMode > 0)
	{
		if(cur.block().length() > wrapMode)
			cur.insertBlock();
	}
	else if(cur.block().length() > maxcol)
	{
		cur.insertBlock();
	}

	// always start at the end just in case someone clicked the window
	//moveCursor(QTextCursor::End);
	// now that we have cursor positioning we can't always start at the end.

	//qDebug(QString(" %1 %2").arg(ch, 2, 16, QChar('0')).arg(QChar(ch)).toAscii());
	//insertPlainText(QString(" %1 ").arg(ch, 2, 16, QChar('0')));
	//insertPlainText(QChar(ch));

	//int _x = cur.columnNumber();
	//int _y = cur.blockNumber();

	//qDebug() << QString("%1 %2 %3 %4 %5").arg(pcmd,2,10,QChar('0')).arg(pcmdx,2,10,QChar('0')).arg(pcmdy,2,10,QChar('0')).arg(ch,2,10,QChar('0')).arg(ch,1,QChar('0'));
	//qDebug() << QString("P %1,%2 X,Y %3,%4 %5").arg(pcmdx,2,10,QChar('0')).arg(pcmdy,2,10,QChar('0')).arg(cur.columnNumber(),2,10,QChar('0')).arg(cur.blockNumber(),2,10,QChar('0')).arg(ch,2,10,QChar('0'));
	switch(pcmd)
	{
	case PCMD_CURPOS_X:
	{
		pcmdx = ch;
//		qDebug() << "CURPOS_X " << pcmdx;
		int j = cur.block().length();
		for(; j <= pcmdx; j++)
		{
			cur.movePosition(QTextCursor::EndOfLine, QTextCursor::MoveAnchor);
			cur.insertText(" ");
		}
		cur.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
		if(pcmdx > 0)
		{
			cur.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, pcmdx);
		}
		setTextCursor(cur);
		pcmd = PCMD_NONE;
//		qDebug() << "CURPOS_X Column" << cur.columnNumber();
	}
	break;

	case PCMD_CURPOS_Y:
	{
		pcmdy = ch;
//		qDebug() << "CURPOS_Y " << pcmdy;
		int j = blockCount();
		for(; j <= pcmdy; j++)
		{
			cur.movePosition(QTextCursor::End);
			cur.insertBlock(cur.blockFormat(), cur.charFormat());
		}
		cur.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
		if(pcmdy > 0) cur.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, pcmdy);
		setTextCursor(cur);
#if 0
		cur.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
		cur.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, pcmdx);
		setTextCursor(cur);
#endif
		pcmd = PCMD_NONE;
//		qDebug() << "CURPOS_Y Column" << cur.columnNumber();
	}
	break;

	case PCMD_CURPOS_XY:
	{
#if 1
		if(pcmdlen == 2)
		{
			pcmdx = ch;
		}
		else if(pcmdlen == 1)
		{
			pcmdy = ch;
			int j = blockCount();
			for(; j <= pcmdy; j++)
			{
				cur.movePosition(QTextCursor::End);
				cur.insertBlock();
			}
			cur.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
			if(pcmdy > 0)
				cur.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, pcmdy);

			j = cur.block().length();
			for(; j <= pcmdx; j++)
			{
				cur.movePosition(QTextCursor::EndOfLine, QTextCursor::MoveAnchor);
				cur.insertText(" ");
			}
			cur.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
			if(pcmdx > 0)
				cur.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, pcmdx);
			setTextCursor(cur);
		}
		pcmdlen--;
		if(pcmdlen < 1)
		{
			pcmd = PCMD_NONE;
		}
#endif
	}
	break;

	default:
	{


		if (ch & 0x80)      //UTF-8 parsing and handling
		{
			if (utfparse == true)
			{

				utf8 <<= 6;
				utf8 |= (ch & 0x3F);

				utfbytes--;

				if (utfbytes == 0)
				{
					utfparse = false;
					cur.insertText(QChar(utf8));
				}
			}
			else
			{
				utfparse = true;
				utf8 = 0;

				while (ch & 0x80)
				{
					ch <<= 1;
					utfbytes++;
				}

				ch >>= utfbytes;

				utf8 = (int)ch;

				utfbytes--;
			}
			return;
		}

		switch(ch)
		{
		case 0x0c:	//EN_ClearScreen:
		{
			if(enableClearScreen)
			{
				setPlainText("");
			}
		}
		break;
		case EN_ClearScreen2:
		{
			if(enableClearScreen16)
			{
				setPlainText("");
			}
		}
		break;

		case EN_HomeCursor:
		{
			if(enableHomeCursor)
			{
				cur.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
				setTextCursor(cur);
			}
		}
		break;

		case EN_PosCursorX:
		{
			if(enablePosCursorX)
			{
				pcmd = PCMD_CURPOS_X;
				pcmdlen  = 1;
			}
		}
		break;

		case EN_PosCursorY:
		{
			if(enablePosCursorY)
			{
				pcmd = PCMD_CURPOS_Y;
				pcmdlen  = 1;
			}
		}
		break;

		case EN_PosXYCursor:
		{
			if(enablePosXYCursor)
			{
				pcmd = PCMD_CURPOS_XY;
				pcmdlen  = 2;
			}
		}
		break;

		case EN_MoveCursorLeft:
		{
			if(enableMoveCursorLeft)
			{
				if(cur.columnNumber() > 0)
				{
					cur.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor);
					setTextCursor(cur);
				}
			}
		}
		break;

		case EN_MoveCursorRight:
		{
			if(enableMoveCursorRight)
			{
				if(cur.columnNumber() >= cur.block().length() - 1)
					cur.insertText(" ");
				cur.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor);
				setTextCursor(cur);
			}
		}
		break;

		case EN_MoveCursorUp:
		{
			if(enableMoveCursorUp)
			{
				if(cur.blockNumber() > 0)
				{
					int col = cur.columnNumber();
//					qDebug() << "MU Column" << col;
					cur.movePosition(QTextCursor::Up, QTextCursor::MoveAnchor);
					int end = cur.block().length();
					for(int n = end; n <= col; n++)
					{
						cur.insertText(" ");
					}
					cur.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
					cur.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, col);
					setTextCursor(cur);
				}
			}
		}
		break;

		case EN_MoveCursorDown:
		{
			if(enableMoveCursorDown)
			{
				int col = cur.columnNumber();
				int row = cur.blockNumber();
				int cnt = blockCount();
				int end = cur.block().length();
//				qDebug() << "MD Column" << col;
				if(row + 1 < cnt)
				{
					cur.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor);
					end = cur.block().length();
					for(int n = end; n <= col; n++)
					{
						cur.insertText(" ");
						cur.movePosition(QTextCursor::EndOfLine, QTextCursor::MoveAnchor);
					}
					end = cur.block().length();
					cur.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
					cur.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, col);
				}
				else
				{
					cur.insertBlock();
					for(int n = 1; n < col; n++)
					{
						cur.insertText(" ");
					}
				}
				setTextCursor(cur);
			}
		}
		break;

		case EN_BeepSpeaker:
		{
			if(enableBeepSpeaker)
			{
				QApplication::beep();
				emit beep();
			}
		}
		break;

		case EN_Backspace:
		{
			if(enableBackspace)
			{
				text = toPlainText();
				setPlainText(text.mid(0, text.length() - 1));
				moveCursor(QTextCursor::End);
			}
		}
		break;

		case EN_Tab:
		{
			if(enableTab)
			{
				int column = cur.columnNumber() % tabsize;
				while(column++ < tabsize)
				{
					moveCursor(QTextCursor::Right, QTextCursor::MoveAnchor);
					cur.insertText(" ");
				}
				setTextCursor(cur);
			}
		}
		break;

		/* fall through case because newline and creturn can change roles
		 * the switch is just to speed things up
		 */
		case EN_NewLine: // fall through
		case EN_CReturn:
		{
			if(ch == SPIN_NEWLINE)
			{
				if(enableNewLine)
				{
					int row = cur.blockNumber();
					int max = blockCount() - 1;
					if(row < max)
					{
						// insert a newline
						cur.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
						text = cur.selectedText();
						cur.removeSelectedText();
						cur.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor);
						cur.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
						cur.insertText(text);
					}
					else
					{
						cur.insertBlock();
					}
					setTextCursor(cur);
				}
			}
			else if(ch == SPIN_RETURN)
			{
				if(enableCReturn)
				{
					text = toPlainText();
					cur.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
					setTextCursor(cur);
				}
			}
		}
		break;

		case EN_ClearToEOL:
		{
			if(enableClearToEOL)
			{
				int end = cur.block().length();
				int col = cur.columnNumber();
				if(end > col)
				{
					cur.clearSelection();
					cur.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, end - col - 1);
					text = cur.selectedText();
					col = cur.columnNumber();
					if(text.length())
					{
						cur.removeSelectedText();
						col = cur.columnNumber();
						text = cur.selectedText();
						col = cur.columnNumber();
						setTextCursor(cur);
						col = cur.columnNumber();
					}
				}
			}
		}
		break;

//		case EN_ClearLinesBelow:
//		{
//			if(enableClearLinesBelow)
//			{
//				cur.movePosition(QTextCursor::Down, QTextCursor::KeepAnchor);
//				cur.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
//				//qDebug() << "Selected " << cur.selectedText();
//				cur.removeSelectedText();
//			}
//		}
//		break;

		default:
		{
			if(cur.block().length() - 1 > cur.columnNumber())
				cur.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
			cur.insertText(QString(ch));
			if (ch == '?')
				emit beep();
			setTextCursor(cur);
		}
		break;
		}
		break;

	} // end pcmd default
	} // end pcmd switch
	//qDebug() << "End Row   " << cur.blockNumber();
	//qDebug() << "End Column" << cur.columnNumber();
	return;
}


void Console::closeEvent(QCloseEvent *ev)
{
	if (m_close)
	{
		ev->accept();
		Config::setValue("ui_pos/console", geometry());
	}
	else
	{
		ev->ignore();
	}
}
