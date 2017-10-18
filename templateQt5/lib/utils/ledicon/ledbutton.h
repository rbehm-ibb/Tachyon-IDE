// ******************************************************
// * copyright (C) 2016 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 5/6/2016 by behm
// * ,
// ******************************************************

#ifndef LEDBUTTON_H
#define LEDBUTTON_H

#include "lediconsource.h"

class LedButton : public QPushButton
{
	Q_OBJECT
public:
	LedButton(LedIconSource::LedColor color, const QString &text, QWidget *parent = 0);
	LedIconSource::LedColor led() const { return m_color; }
public slots:
	void setLed(LedIconSource::LedColor color);
	void setOnOff(bool on);
	void setChecked(bool on);
private:
	LedIconSource::LedColor m_color;
};

#endif // LEDBUTTON_H
