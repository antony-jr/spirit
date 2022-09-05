#include <QPainter>
#include <QScreen>
#include <QGuiApplication>
#include <QApplication>

#include "spirit.hpp"

struct Coordinates {
   int x = 0,
       y = 0;
};

static Coordinates getCoordinates(short pos, QRect geometry, QRect frame, int w, int h, int yoff, int _yoff) {
   w += 40;

   auto point = geometry.topLeft();
   auto x = point.x();
   auto y = point.y();

 
   if(pos == Spirit::Position::TopRight) {
     point = geometry.topRight();
     x = point.x();
     y = point.y();

     x = x + frame.width() - (w*2);
     y = y - frame.height() + (yoff < 0 ? 0 : yoff);
   } else if(pos == Spirit::Position::BottomLeft) {
     point = geometry.bottomLeft();
     x = point.x();
     y = point.y();

     x = x - frame.width() + w;
     y = y - frame.height() - (_yoff < 0 ? 0 : _yoff);

   } else if(pos == Spirit::Position::BottomRight) {
     point = geometry.bottomRight();
     x = point.x();
     y = point.y();

     x = x - frame.width() - (w/4);
     y = y - frame.height() - (_yoff < 0 ? 0 : _yoff);
   } else { // TopLeft
      x += w;
      y = y - frame.height() + (yoff <= 0 ? 0 : yoff);
   }

   Coordinates r;
   r.x = x;
   r.y = y;

   return r;
}

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

void Spirit::update(QRect geometry) {
   if (m_Movie.isNull() || b_ClearRequested) {
      return;
   }

   auto pos = getCoordinates(n_Position, geometry, frameGeometry(), width(), height(), n_YOff, n__YOff);
   auto x = pos.x;
   auto y = pos.y;

   if ( y < 0 || x < 0) {
      if(!b_Flipped && m_Variant != nullptr) {
	 qDebug() << "FLIP";
	 b_Flipped = true;
	 hide();
	 m_Movie->stop();
	 m_Movie.reset(new QMovie);
	 setMovie(m_Movie.data());

	 m_Variant->reset(); 
	 m_Movie->setDevice(m_Variant);	 
	 m_Movie->setSpeed(n_Speed); 
	 
	 
	QObject::connect(m_Movie.data(), &QMovie::frameChanged, this, &Spirit::handleFrameChanged);
	 m_Movie->start();
	 show();
      }
	 pos = getCoordinates(Position::BottomRight, geometry, frameGeometry(), width(), height(), n_YOff, n__YOff);
      
        x = pos.x;
        y = pos.y;
   } else {
      if(b_Flipped) {
	 qDebug() << "UNFLIP";
	 b_Flipped = false;
	 hide();
	 m_Movie->stop();
	 m_Movie.reset(new QMovie);
	 setMovie(m_Movie.data());

	 m_Buffer->reset();
	 m_Movie->setDevice(m_Buffer);
	 m_Movie->setSpeed(n_Speed); 
	 
	QObject::connect(m_Movie.data(), &QMovie::frameChanged, this, &Spirit::handleFrameChanged); 
	      m_Movie->start();

	      show();

	 qDebug() << "Width: " << width() << "Height: " << height();

  pos = getCoordinates(n_Position, geometry, frameGeometry(), width(), height(), n_YOff, n__YOff);
    x = pos.x;
    y = pos.y;
      }
   }

   qDebug() << "X: " << x << ", Y: " << y;

   move(x,y);
   show();
}

void Spirit::animate(QString action, 
	      	     QBuffer *buffer,
		     QBuffer *_buffer,
	      	     QBuffer *play,
	      	     bool loop,
		     int scale,
		     int speed,
		     QString nxt,
		     QVector<int> offsets) {
  hide();
  n_XOff = offsets[0];
  n_YOff = offsets[1];
  n__XOff = offsets[2];
  n__YOff =   offsets[3];

  b_Loop = loop;
  n_Scale = scale;
  n_Speed = speed;
  m_Action = action;
  m_Next = nxt;

  qDebug() << "Next: " << m_Next;
  qDebug() << "Loop: " << b_Loop;

  if(m_Buffer) {
     m_Buffer->close();
  }

  if(m_Variant) {
     m_Variant->close();
  }

  m_Buffer = buffer;
  m_Variant = _buffer;

  if(m_Buffer && !m_Buffer->isOpen()) {
    m_Buffer->open(QIODevice::ReadOnly); 
  }

  if(m_Variant && !m_Variant->isOpen()) {
     m_Variant->open(QIODevice::ReadOnly);
  }

  m_Buffer->reset();
  if(m_Variant) {
  m_Variant->reset();
  }

  if(!m_Movie.isNull()) {
     m_Movie->stop();
  }
  m_Movie.reset(new QMovie);
  setMovie(m_Movie.data());

  QObject::connect(m_Movie.data(), &QMovie::frameChanged, this, &Spirit::handleFrameChanged);


  if(n_Position == Position::TopRight ||
     n_Position == Position::TopLeft) {
     m_Movie->setDevice(m_Buffer);
  } else {
     if(m_Variant != nullptr) {
	m_Movie->setDevice(m_Variant);
     } else {
	m_Movie->setDevice(m_Buffer);
     }
  }

  m_Movie->setSpeed(n_Speed); 
  m_Movie->start();
  show();
}

void Spirit::clear() {
   b_ClearRequested = true;
   hide();
	 b_Loop = false; 
	 n_Scale = n_Speed = 100;
	 n_Position = Position::TopLeft;
	 m_Action = "default";
	 m_Next = "";
	 
	 if(m_Buffer) {
	    m_Buffer->close();
	 }
	 
	 if(m_Variant) {
	    m_Variant->close();
	 }
	 
	 m_Buffer = m_Variant = nullptr;
	 if(!m_Movie.isNull()) {
	    m_Movie->stop();
	 }
	
	 m_Movie.reset(new QMovie);
	 setMovie(m_Movie.data());
	
	b_ClearRequested = false; 
	 emit cleared();

}

void Spirit::handleMovieStarted() {

}

void Spirit::handleMovieFinished() {
   qDebug() << "Movie Finished.";
   if(!b_Loop) {
      //m_Movie->stop();
      emit requestAction(m_Next);
   }
}

void Spirit::handleFrameChanged(int frame) {
   if(m_Movie.isNull()) {
      return;
   }

   if(frame + 1 == m_Movie->frameCount()) {
      if(!b_Loop) {
	 emit requestAction(m_Next);
      }
   }
}
