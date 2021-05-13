#include <QApplication>
#include <QWindow>
#include <QProcess>
#include <QtWidgets/QLabel>
#include <QDebug>

#include "windowinfo.hpp"
//#include "spiritmanager.hpp"
#include "spirit.hpp"
#include "xdo_wrapper.hpp"

int main(int ac, char **av) {
	if(ac == 1) {
		return 0;
	}
	QApplication app(ac, av);
	QApplication::setQuitOnLastWindowClosed(false);

	QProcess proc;
	proc.setProgram(av[1]);
	QObject::connect(&proc,
			 QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
			 &app,
			 &QApplication::quit);
	Spirit s;
	s.setGraphic(":/default.webp", false);

	proc.start();
	WindowInfo info((int)proc.processId());
	//SpiritManager manager(":/default.webp", ":/default.gif");

	QObject::connect(&info, &WindowInfo::focused, &s, &Spirit::update);
	QObject::connect(&info, &WindowInfo::unFocused, &s, &Spirit::onTop);
	QObject::connect(&proc, &QProcess::started, &info, &WindowInfo::start);
	return app.exec();
}
