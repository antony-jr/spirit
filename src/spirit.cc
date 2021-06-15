#include <QRect>
#include <QPixmap>
#include <QMovie>
#include <QPair>

#include "spirit.hpp"

static QPair<int,int> optimalSize(const QPixmap &pix, int w_thresh, int h_thresh) {
	QPair<int,int> r;
	r.first = 0;
	r.second = 0;

	auto pw = pix.width(),
	     ph = pix.height();

	if(pw < w_thresh || ph < h_thresh) {
		r.first = w_thresh;
		r.second = h_thresh;
		return r;
	}

	int w_factor = 2,
	    h_factor = 2;

	while(pw / w_factor > w_thresh ||
	      ph / h_factor > h_thresh) {
		w_factor++;
		h_factor++;
	}

	pw = pw / w_factor;
	ph = ph / h_factor;

	r.first = pw;
	r.second = ph;

	return r;
}

Spirit::Spirit()
	: QLabel("", nullptr, Qt::FramelessWindowHint |
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
	resize(w, h);
}

Spirit::~Spirit() {
	hide();
}

void Spirit::setWidth(int value) {
	w = value;
}

void Spirit::setHeight(int value) {
	h = value;
}

void Spirit::setGraphic(const QString &file) {
	QMovie *m = new QMovie(this);
	m->setFileName(file);
	m->start();

	auto pix = m->currentPixmap();
	//auto s = optimalSize(pix, w, h);
	//m->setScaledSize(QSize(s.first, s.second));
	
	setMovie(m);
	//resize(s.first, s.second);
	resize(pix.width(), pix.height());
}

void Spirit::update(QRect geometry) {
	auto point = geometry.topLeft();

	move(point.x() + 100, point.y() - (frameRect().height()) + 46);
	show();

	/*
	LOG_START "Spirit::update(" 
	LOG_REDIR xpos 
	LOG_REDIR "," 
	LOG_REDIR ypos 
	LOG_REDIR "): "
	LOG_REDIR "Moved x->" 
	LOG_REDIR x()
	LOG_REDIR ", y->"
	LOG_REDIR y()
	LOG_REDIR "."
	LOG_END;
	*/
}

