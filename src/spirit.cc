#include <QPixmap>
#include <QDebug>
#include "spirit.hpp"

Spirit::Spirit()
	: QLabel("", nullptr, 
		 Qt::FramelessWindowHint |
		 Qt::Tool |
		 Qt::WindowStaysOnTopHint) {
		
	setAttribute(Qt::WA_TranslucentBackground);
        setStyleSheet(QString::fromUtf8("background: transparent;"));
	resize(100, 100);

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
	}
	setScaledContents(true);
}

Spirit::~Spirit() {
	hide();
}

void Spirit::update(int x, int y, unsigned w, unsigned h) {
	move(x + 100, y - ((height()/2) + 100));
	show();
}
