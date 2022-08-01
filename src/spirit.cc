#include <QPainter>

#include "spirit.hpp"

Spirit::Spirit()
	: QLabel(nullptr, Qt::FramelessWindowHint |
			   Qt::Tool |
			   Qt::WindowStaysOnTopHint |
			   Qt::WindowTransparentForInput |
			   Qt::WindowDoesNotAcceptFocus | 
			   Qt::NoDropShadowWindowHint |
			   Qt::X11BypassWindowManagerHint) {
		
	setAttribute(Qt::WA_TranslucentBackground, true);
        setAttribute(Qt::WA_TransparentForMouseEvents, true);
	setAttribute(Qt::WA_ShowWithoutActivating, true);
	setAttribute(Qt::WA_X11DoNotAcceptFocus, true);
	setStyleSheet(QString::fromUtf8("background: transparent; border: none;"));

	m_Movie.reset(new QMovie);
	setMovie(m_Movie.data());
}

Spirit::~Spirit() {
   disconnect();
   hide();
}

void Spirit::setScale(int scale) {
   n_Scale = scale;
}

void Spirit::setSpeed(int speed) {
   n_Speed = speed;
}

void Spirit::setPosition(short pos) {
   n_Position = pos; 
}

void Spirit::setYOffset(int yoff) {
   n_YOff = yoff;
}

void Spirit::update(QRect geometry) {
   auto point = geometry.topLeft();
   auto w = width() + 40;
   auto h = height();
   auto x = point.x();
   auto y = point.y();

   if(n_Position == Position::TopRight) {
     point = geometry.topRight();
     x = point.x();
     y = point.y();

     x = x + frameGeometry().width() - (w*2);
     y = y - frameGeometry().height() + (n_YOff < 0 ? 0 : n_YOff);
   } else if(n_Position == Position::BottomLeft) {
     point = geometry.bottomLeft();
     x = point.x();
     y = point.y();
 
     x = x - frameGeometry().width() + w;
     y = y - frameGeometry().height() - (n_YOff < 0 ? 0 : n_YOff); 
  
   } else if(n_Position == Position::BottomRight) {
     point = geometry.bottomRight();
     x = point.x();
     y = point.y();

     x = x - frameGeometry().width() - (w/2);
     y = y - frameGeometry().height() - (n_YOff < 0 ? 0 : n_YOff); 

   } else {
      x += w; 
      y = y - frameGeometry().height() + (n_YOff < 0 ? 0 : n_YOff);
   }

   move(x,y);
   show();
}

void Spirit::animate(QString action, 
	      	     QBuffer *buffer,
	      	     QBuffer *play,
	      	     bool loop,
		     int scale,
		     int speed,
		     QString nxt) {
  b_Loop = loop;
  n_Scale = scale;
  n_Speed = speed;
  m_Next = nxt;

  m_Movie->stop();
  m_Movie->setDevice(buffer);
  m_Movie->setSpeed(n_Speed); 
  m_Movie->start();
}

void Spirit::handleMovieStarted() {

}

void Spirit::handleMovieFinished() {
   if(!b_Loop) {
      //m_Movie->stop();
      emit requestAction(m_Next);
   }
}
