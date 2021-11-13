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
}

Spirit::~Spirit() {
   disconnect();
   hide();
}

void Spirit::setPosition(short pos) {
   n_Position = pos; 
}

void Spirit::setYOffset(int yoff) {
   n_YOff = yoff;
}

#include <QDebug> 

void Spirit::update(QRect geometry) {
   qDebug() << "update";

   auto point = geometry.topLeft();
   auto w = width() + 40;
   auto h = height();
   auto x = point.x();
   auto y = point.y();

   if(n_Position == Position::TopRight) {
     point = geometry.topRight();
     x = point.x();
     y = point.y();

     x = x + frameGeometry().width() - w;
     y = y - frameGeometry().height() + (n_YOff < 0 ? 0 : n_YOff);
   } else if(n_Position == Position::BottomLeft) {
     point = geometry.bottomLeft();
     x = point.x();
     y = point.y();
     
     x += w;
     y = y + frameGeometry().height() - h + (n_YOff < 0 ? 0 : n_YOff); 
   } else if(n_Position == Position::BottomRight) {
     point = geometry.bottomRight();
     x = point.x();
     y = point.y();

     x = x + frameGeometry().width() - w;
     y = y + frameGeometry().height() - h + (n_YOff < 0 ? 0 : n_YOff); 
   } else {
      x += w; 
      y = y - frameGeometry().height() + (n_YOff < 0 ? 0 : n_YOff);
   }

   move(x,y);
   show();
}

void Spirit::capture(QBuffer *data) {
   /// The QBuffer pointer ownership is not with us so don't free 
   /// it.

   qDebug() << "capture";

   data->open(QIODevice::ReadOnly);

   auto bytes = data->readAll();
   QPixmap pix;
   pix.loadFromData(bytes);

   data->close();

   if(width() != pix.width() ||
	height() != pix.height()) {
      resize(pix.width(), pix.height());
   }

   clear();
   setPixmap(pix);
}
