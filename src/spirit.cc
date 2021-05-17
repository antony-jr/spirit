#include <QPixmap>
#include <QMovie>
#include <QPair>
#include <QDebug>

#include "spirit.hpp"
#include "windowinfo.hpp"

Qt::WindowFlags flags = Qt::FramelessWindowHint |
			Qt::Tool |
			Qt::WindowStaysOnTopHint |
			Qt::WindowTransparentForInput |
			Qt::WindowDoesNotAcceptFocus;

static QPair<int,int> optimalSize(const QPixmap &pix, int w_thresh, int h_thresh) {
	QPair<int,int> r;
	r.first = 0;
	r.second = 0;
	
	int w_factor = 2,
	    h_factor = 2;

	while(pix.width() / w_factor > w_thresh ||
	      pix.height() / h_factor > h_thresh) {
		w_factor++;
		h_factor++;
	}

	int pw = pix.width() / w_factor;
	int ph = pix.height() / h_factor;

	r.first = pw;
	r.second = ph;

	return r;
}

Spirit::Spirit()
	: QLabel("", nullptr, flags) {
		
	setAttribute(Qt::WA_TranslucentBackground, true);
        setAttribute(Qt::WA_TransparentForMouseEvents, true);
	setAttribute(Qt::WA_ShowWithoutActivating, true);
	setAttribute(Qt::WA_X11DoNotAcceptFocus, true);
	setStyleSheet(QString::fromUtf8("background: transparent;"));
	resize(w, h);
}

Spirit::~Spirit() {
	hide();
}

void Spirit::onTop() {
#if 0
	if(windowFlags() == flags) {
		setWindowFlags(flags ^ Qt::WindowStaysOnTopHint);
	}
	show();
#endif
}

void Spirit::setXOffset(int value) {
	xoff = value;
}

void Spirit::setYOffset(int value) {
	auto sender = qobject_cast<WindowInfo*>(QObject::sender());
	if(!sender) {
		guess = false;
	}else {
		if(!guess) {
			return;
		}
	}
	yoff = value;
}

void Spirit::setWidth(int value) {
	w = value;
}

void Spirit::setHeight(int value) {
	h = value;
}

void Spirit::setDebug(bool value) {
	debug = value;
}

void Spirit::setGraphic(const QString &file, bool is_png) {
	hide();
	QPair<int, int> s;

	if(is_png) {
		QPixmap pixmap(file);
		setPixmap(pixmap);
		setScaledContents(true);
		s = optimalSize(pixmap, w, h);
	}else {
		QMovie *movie = new QMovie(this);
		movie->setFileName(file);
		setMovie(movie);	
		movie->start();
		
		auto pix = movie->currentPixmap();
		s = optimalSize(pix, w, h);
		movie->setScaledSize(QSize(s.first, s.second));
	}	
	resize(s.first, s.second);
}

void Spirit::update(int xpos, int ypos, unsigned w, unsigned h) {
	move(xpos + (w/width()) + int(width() * 0.5f) + xoff, 
	     ypos - int(height()*0.75f) - yoff);
	show();

	if(debug) {
		qDebug() << "Spirit:: X: "
			 << x()
			 << " Y: "
			 << y();
	}

	if(y() < 0 || x() < 0) {
		hide();
	}
}
