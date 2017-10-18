// ******************************************************
// * copyright (C) 2016 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 5/6/2016 by behm
// * ,
// ******************************************************

#include "ledicon.h"

LedIcon::LedIcon(QWidget *parent)
	: QLabel(parent)
{
	QPixmap pm = LedIconSource::pix(LedIconSource::Off);
	m_size = pm.width();
	setLed(LedIconSource::Off);
}

LedIcon::LedIcon(LedIconSource::LedColor color, QWidget *parent)
	: QLabel(parent)
{
	setLed(color);
}

void LedIcon::setLed(LedIconSource::LedColor color)
{
	setPixmap(LedIconSource::pix(color).scaled(m_size, m_size));
	m_color = color;
}

void LedIcon::setOnOff(bool on)
{
	setPixmap(LedIconSource::pix(on).scaled(m_size, m_size));
}



void LedIcon::setSize(int size)
{
	m_size = size;
	setLed(m_color);
}



