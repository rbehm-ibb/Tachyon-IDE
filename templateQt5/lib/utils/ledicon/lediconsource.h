// ******************************************************
// * copyright (C) 2016 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// ******************************************************

#ifndef LEDICONSOURCE_H
#define LEDICONSOURCE_H

#include "project.h"

///
/// \brief The LedIcon class supplies a LED symbols to other classes
///
/// It is implemented as a singleton to be used from everywhere.

class LedIconSource
{
	Q_GADGET
public:
	/// \brief The LedColor enum enumerates the defiedn and usable colors.
	enum LedColor {
		None,	///< this is an empty icon/pixmap
		Off,	///< the off symbol, gray
		On,	///< default green, can be set by setOnColor()
		Green,	///< green led
		Red,	///< red led
		Yellow,	 ///< yellow led
		Blue	///< blue led
	};
#if QT_VERSION < QT_VERSION_CHECK(5, 5, 0)
	Q_ENUMS(LedColor)
#else
	Q_ENUM(LedColor)
#endif
	static QIcon icon(LedColor color);	///< get the icon with color
	static QIcon icon(bool on);		///< get icon for state on/off
	static QPixmap pix(LedColor color);	///< get the icon with color
	static QPixmap pix(bool on);		///< get icon for state on/off
	static const QList<LedIconSource::LedColor> colors();	///< a list with all values of LedColor.
	static void setOnColor(LedColor color);	///< which color will be used for "on", initial = Green
	static LedColor onColor();		///< the currently set color for On state
private:
	LedIconSource();			///< internal _ct_ only used in i()
	void setOnColorIntern(LedColor color);	///< helper forsetOnColor
	QMap<LedColor, QPixmap> m_pixmaps;	///< maps colors to pixmaps
	QMap<LedColor, QIcon> m_icons;		///< maps colors to icons
	LedColor m_onColor;			///< currently defined color for on. \sa onColor(), setOnColor()
	const QList<LedIconSource::LedColor> m_colors; ///< the list of all values for LedColor
	static LedIconSource *instance;		///< the single instance.
	static LedIconSource *i();		///< access to the singleton
};

#endif // LEDICON_H
