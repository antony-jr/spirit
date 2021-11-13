#ifndef SPIRIT_HPP_INCLUDED
#define SPIRIT_HPP_INCLUDED
#include <QRect>
#include <QPixmap>
#include <QLabel>
#include <QBuffer>

#include "spiritenums.hpp"

/// This is the GUI Widget for 
/// the spirit.
/// This runs in the main thread and all 
/// heavy lifting is done in the spirit worker which
/// runs all calculations and parsing in a separate 
/// thread.
class Spirit : public QLabel {
	Q_OBJECT
public:
	struct Position : public SpiritEnums::Spirit::Position { };

	Spirit();
	~Spirit();
public Q_SLOTS:
   	void setPosition(short);
	void setYOffset(int);

   	// Moves the widget to the given rect.
   	void update(QRect);
	
	// Given a frame, draw it. 
	void capture(QBuffer*);
private:
	short n_Position = Position::TopLeft;
	int n_YOff = -1,
	    n_XOff = -1;

	QPixmap m_CurrentFrame;
};

#endif // SPIRIT_HPP_INCLUDED
