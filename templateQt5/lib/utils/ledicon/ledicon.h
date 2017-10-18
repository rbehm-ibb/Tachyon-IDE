// ******************************************************
// * copyright (C) 2016 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 5/6/2016 by behm
// ******************************************************

#ifndef LEDICON_H
#define LEDICON_H

#include "lediconsource.h"

///
/// \brief The LedIcon class is a QLabel to show a "LED"
///
/// It can be placed anywhere (e.g. into QToolBar) to show states.
/// It has two modes:
/// - show different colored LEDs by using setLed()
/// - show On/Off state
///
/// Colors for LEDs are defined by LedIconSource::LedColor
/// The pixmaps for the LEDs are com from LedIconSource
///
class LedIcon : public QLabel
{
	Q_OBJECT
public:
	LedIcon(QWidget *parent = 0);
	/// \brief Create a LedIcon
	/// \param color the initial color to use
	/// \param parent the usaula parent for Qidget
	LedIcon(LedIconSource::LedColor color = LedIconSource::Off, QWidget *parent = 0);
	LedIconSource::LedColor color() const { return m_color; }

	int size() const { return m_size; }
	void setSize(int size);

public slots:
	void setLed(LedIconSource::LedColor color);	///< show a LED with \param color
	void setOnOff(bool on);		///< show a LED with on or off state
private:
	LedIconSource::LedColor m_color;
	int m_size;
};

#endif // LEDICON_H
