// ******************************************************
// * copyright (C) 2015 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 2/19/2016 by behm
// ******************************************************

#include "lediconsource.h"

LedIconSource * LedIconSource::instance = nullptr;

LedIconSource *LedIconSource::i()
{
	if (instance == nullptr)
	{
		instance = new LedIconSource;

	}
	return instance;
}

QIcon LedIconSource::icon(LedIconSource::LedColor color)
{
	return i()->m_pixmaps[color];
}

QIcon LedIconSource::icon(bool on)
{
	return i()->m_icons[on ? On : Off];
}

QPixmap LedIconSource::pix(LedIconSource::LedColor color)
{
	return i()->m_pixmaps[color];
}

QPixmap LedIconSource::pix(bool on)
{
	return i()->m_pixmaps[on ? On : Off];
}

const QList<LedIconSource::LedColor> LedIconSource::colors()
{
	return i()->m_colors;
}

void LedIconSource::setOnColor(LedIconSource::LedColor color)
{
	i()->setOnColorIntern(color);
}

LedIconSource::LedIconSource()
	: m_colors( { LedIconSource::None, LedIconSource::Off, LedIconSource::On,
		    LedIconSource::Green, LedIconSource::Red,
		    LedIconSource::Yellow, LedIconSource::Blue } )
{
	QPixmap none;
	QPixmap off(":/led/LED_off.png");
	QPixmap green(":/led/LED_green.png");
	QPixmap yellow(":/led/LED_yellow.png");
	QPixmap red(":/led/LED_red.png");
	QPixmap blue(":/led/LED_blue.png");

	m_pixmaps.insert(None, none);
	m_pixmaps.insert(Off, off);
	m_pixmaps.insert(On, green);
	m_pixmaps.insert(Green, green);
	m_pixmaps.insert(Yellow, yellow);
	m_pixmaps.insert(Red, red);
	m_pixmaps.insert(Blue, blue);
	m_onColor = Green;
	foreach (const LedColor c, m_colors)
	{
		m_icons.insert(c, QIcon(m_pixmaps[c]));
	}
}

void LedIconSource::setOnColorIntern(LedIconSource::LedColor color)
{
	if (m_onColor != color)
	{
		m_onColor = color;
		m_pixmaps[On] = m_pixmaps[color];
		m_icons[On] = m_icons[color];
	}
}


