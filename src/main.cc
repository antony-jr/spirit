#include <QApplication>
#include <QtWidgets/QLabel>
#include <QDebug>

#include "windowinfo.hpp"

int main(int ac, char **av) {
	QApplication app(ac, av);
	QApplication::setQuitOnLastWindowClosed(false);

	WindowInfo info;

	QLabel label("Hello World!", nullptr, Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
	label.setAttribute(Qt::WA_TranslucentBackground);
        label.setStyleSheet(QString::fromUtf8("background: rgba(0,0,0,1); color: white;"));
        label.show();
	label.resize(30, 30);


	QObject::connect(&info, &WindowInfo::update,
	[&](int x, int y, unsigned w, unsigned h) {
		qDebug() << "X: " << x << ", Y: " << y;
		qDebug() << "Width: " << w << ", Height: " << h;
		label.show();	
	});

	QObject::connect(&info, &WindowInfo::hintHide,
	[&]() {
		label.hide();
	});


	return app.exec();
}
