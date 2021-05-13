//#include <QPixmap>
#include <QMovie>
#include <QDebug>
#include "spirit.hpp"

Spirit::Spirit()
	: QLabel("", nullptr, 
		 Qt::FramelessWindowHint |
		 Qt::Tool |
		 Qt::WindowStaysOnTopHint) {
		
	setAttribute(Qt::WA_TranslucentBackground);
        setStyleSheet(QString::fromUtf8("background: transparent;"));
	resize(200, 200);

	/*
	QPixmap pixmap(":/default.png");
	if(pixmap.isNull()){
		qDebug() << "Pixmap is null.";
	}
	setPixmap(pixmap);

	if(pixmap.width() < pixmap.height()) {
		/// potrait
		resize(100, 200);
	}else {
		resize(200, 100);
	}*/
	//setScaledContents(true);


	QMovie *movie = new QMovie(QString::fromUtf8(":/default.gif"));
	setMovie(movie);	
	movie->start();

	auto pix = movie->currentPixmap();
	movie->stop();

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

	resize(pw, ph);
	movie->setScaledSize(QSize(pw, ph));
	movie->start();
}

Spirit::~Spirit() {
	hide();
}

void Spirit::update(int x, int y, unsigned w, unsigned h) {
	move(x + 90, y - 200);
	show();
}
