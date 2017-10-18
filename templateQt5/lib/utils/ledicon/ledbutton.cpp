// ******************************************************
// * copyright (C) 2016 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 5/6/2016 by behm
// * ,
// ******************************************************

#include "ledbutton.h"

LedButton::LedButton(LedIconSource::LedColor color, const QString &text, QWidget *parent)
	: QPushButton(text, parent)
{
	setLed(color);
}

void LedButton::setLed(LedIconSource::LedColor color)
{
	m_color = color;
	setIcon(LedIconSource::icon(color));
}

void LedButton::setOnOff(bool on)
{
	setIcon(LedIconSource::icon(on));
}

void LedButton::setChecked(bool on)
{
	QPushButton::setChecked(on);
	setOnOff(on);
}


