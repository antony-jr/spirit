#include <QPixmap>
#include <QMovie>
#include <QPair>
#include <QDebug>
#include "spirit.hpp"

Qt::WindowFlags flags = Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint;

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
		
	setAttribute(Qt::WA_TranslucentBackground);
        setAttribute(Qt::WA_TransparentForMouseEvents);
	setAttribute(Qt::WA_ShowWithoutActivating);	
	setStyleSheet(QString::fromUtf8("background: transparent;"));
	resize(w, h);
}

Spirit::~Spirit() {
	hide();
}

void Spirit::onTop() {
	if(windowFlags() == flags) {
		setWindowFlags(flags ^ Qt::WindowStaysOnTopHint);
	}
	show();
}

void Spirit::setXOffset(int value) {
	xoff = value;
}

void Spirit::setYOffset(int value) {
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
	move(xpos + xoff, ypos - yoff);
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
