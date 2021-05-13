#include <QApplication>
#include <QtWidgets/QLabel>
#include <QDebug>

#include "windowinfo.hpp"
#include "spirit.hpp"

int main(int ac, char **av) {
	QApplication app(ac, av);
	QApplication::setQuitOnLastWindowClosed(false);

	WindowInfo info;
	Spirit spirit;

	QObject::connect(&info, &WindowInfo::update, &spirit, &Spirit::update);
	QObject::connect(&info, &WindowInfo::hintHide, &spirit, &Spirit::hide);
	return app.exec();
}
