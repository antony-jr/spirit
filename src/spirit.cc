#include <QPixmap>
#include <QMovie>
#include <QPair>
#include "spirit.hpp"

Qt::WindowFlags flags = Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint;

static QPair<int,int> optimalSize(const QPixmap &pix) {
	QPair<int,int> r;
	r.first = 0;
	r.second = 0;
	
	int w_factor = 2,
	    y_factor = 2;

	int thresh = 240;
	while(pix.width() / w_factor > thresh ||
	      pix.height() / y_factor > thresh) {
		w_factor++;
		y_factor++;
	}

	int pw = pix.width() / w_factor;
	int ph = pix.height() / y_factor;

	r.first = pw;
	r.second = ph;

	return r;
}

Spirit::Spirit()
	: QLabel("", nullptr, flags) {
		
	setAttribute(Qt::WA_TranslucentBackground);
        setAttribute(Qt::WA_TransparentForMouseEvents);	
	setStyleSheet(QString::fromUtf8("background: transparent;"));
	resize(240, 240);
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

void Spirit::setGraphic(const QString &file, bool is_png) {
	QPair<int, int> s;

	if(is_png) {
		QPixmap pixmap(file);
		setPixmap(pixmap);
		setScaledContents(true);
		s = optimalSize(pixmap);
	}else {
		QMovie *movie = new QMovie(this);
		movie->setFileName(file);
		setMovie(movie);	
		movie->start();
		
		auto pix = movie->currentPixmap();
		s = optimalSize(pix);
		movie->setScaledSize(QSize(s.first, s.second));
	}	
	resize(s.first, s.second);
}

void Spirit::update(int x, int y, unsigned w, unsigned h) {
	if(windowFlags() == flags ^ Qt::WindowStaysOnTopHint) {	
		setWindowFlags(flags);	
	}
	hide();
	show();
	move(x + 90, y - 200);
	
	raise();
}
