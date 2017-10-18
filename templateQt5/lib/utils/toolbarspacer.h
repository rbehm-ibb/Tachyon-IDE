// ******************************************************
// * copyright (C) 2016 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 5/6/2016 by behm
// * ,
// ******************************************************

#ifndef TOOLBARSPACER_H
#define TOOLBARSPACER_H

///
/// \brief The ToolBarSpacer class is a helper to put spacers into QToolBar%s
///
/// QToolBar does not use the usual layout system and therefore lacks functions
/// like addStretch() or addSpacing()
///
/// With ToolBarSpacer this can be achieved. the simplest way is to use
/// addSpacer(). It internally creates a ToolBarSpacer and adds it to the toolbar.
///
/// If width if <= 0 (the default) it creates a spacer that acts like stretch.
///
class ToolBarSpacer : public QWidget
{
	Q_OBJECT
public:
	/// \brief Creates a ToolBarSpacer
	///
	/// Watch out, it will be created w/o a parent. It is only intened to be added to a QToolBar.
	/// \n Normally you should just use addSpacer().
	/// \param width if > 0 then this sets the fixed width, else it will take all available space.
	explicit ToolBarSpacer(int width = -1);
	/// \brief Creates and adds a ToolBarSpacer
	/// \param tb QToolBar where the ToolBarSpacer is added to.
	/// \param width width if > 0 then this sets the fixed width, else it will take all available space.
	static void addSpacer(QToolBar *tb, int width = -1);
	static void addAbout(QToolBar *tb, QWidget *parent, const char aboutSlot[]);
};

#endif // TOOLBARSPACER_H
