#include <QApplication>
#include <QProcess>
#include <QFileInfo>
#include <QtWidgets/QLabel>

#include "termcolor.hpp"
#include "windowinfo.hpp"
#include "spirit.hpp"
#include "xdo_wrapper.hpp"

static void usage(const char *prog) {
	std::cout << termcolor::bold << "Spirit"
		  << termcolor::reset
		  << " " 
		  << termcolor::underline
		  << "v0.1.0"
		  << termcolor::reset
		  << ","
		  << " Attach gif/webp over any X11 window in a stylish way."
		  << "\n"
		  << termcolor::magenta
		  << termcolor::bold
		  << "D. Antony J.R <antonyjr@pm.me>"
		  << termcolor::reset
		  << "\n\n";

	std::cout << termcolor::bold
		  << "Usage: " << prog << " [GIF/WEBP FILE] [PROGRAM TO EXEC]"
		  << termcolor::reset
		  << "\n\n";

	std::cout << termcolor::blue
		  << termcolor::bold
		  << "Example:\n"
		  << "\t" << prog << " \":/default.webp\" konsole\n"
		  << termcolor::reset;
}

int main(int ac, char **av) {
	if(ac < 2) {
		usage(av[0]);
		return 0;
	}

	QApplication app(ac, av);
	QApplication::setQuitOnLastWindowClosed(false);

	QProcess proc;
	proc.setProgram(av[2]);
	
	QObject::connect(&proc,
			 QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
			 &app,
			 &QApplication::quit);

	
	QString filename(av[1]);
	QFileInfo finfo(filename);

	if(!finfo.isFile() || !finfo.exists()) {
		std::cout << termcolor::red << termcolor::bold << "Fatal: cannot open file" << av[1] 
			  << termcolor::reset << "\n";	
		return -1;
	}

	Spirit s;
	s.setGraphic(filename, false);

	proc.start();
	WindowInfo info((int)proc.processId());

	QObject::connect(&info, &WindowInfo::focused, &s, &Spirit::update);
	QObject::connect(&info, &WindowInfo::unFocused, &s, &Spirit::onTop);
	QObject::connect(&info, &WindowInfo::hintHide, &s, &Spirit::hide);
	QObject::connect(&proc, &QProcess::started, &info, &WindowInfo::start);
	return app.exec();
}
