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
		  << "Usage: " << prog << " [OPTIONS] [GIF/WEBP FILE] [PROGRAM TO EXEC]"
		  << termcolor::reset
		  << "\n\n";

	std::cout << "Options:\n"
		  << " -d,--debug " << "\t" << "enable debug messages.\n\n";

	std::cout << termcolor::blue
		  << termcolor::bold
		  << "Example:\n"
		  << "\t" << prog << " \":default.gif\" konsole\n"
		  << termcolor::reset;
}

int main(int ac, char **av) {
	if(ac < 3) {
		usage(av[0]);
		return 0;
	}

	QApplication app(ac, av);
	QApplication::setQuitOnLastWindowClosed(false);

	char *program_exec = NULL,
	     *giffile = NULL;
	bool debug = false;
	
	if(ac > 3) {
		if(strstr(av[1], "-d") ||
		   strstr(av[1], "--debug")) {
			debug = true;
		}else {
			usage(av[0]);
			return 0;
		}

		program_exec = av[3];
		giffile = av[2];
	}else {
		program_exec = av[2];
		giffile = av[1];
	}

	QProcess proc;
	proc.setProgram(program_exec);
	
	QObject::connect(&proc,
			 QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
			 &app,
			 &QApplication::quit);

	
	QString filename(giffile);
	QFileInfo finfo(filename);

	if(!finfo.isFile() || !finfo.exists()) {
		std::cout << termcolor::red << termcolor::bold << "Fatal: cannot open file " << av[1] 
			  << termcolor::reset << "\n";	
		return -1;
	}

	Spirit s;
	s.setGraphic(filename, false);
	s.setDebug(debug);

	proc.start();
	WindowInfo info((int)proc.processId());
	info.setDebug(debug);

	QObject::connect(&info, &WindowInfo::focused, &s, &Spirit::update);
	QObject::connect(&info, &WindowInfo::unFocused, &s, &Spirit::onTop);
	QObject::connect(&info, &WindowInfo::hintHide, &s, &Spirit::hide);
	QObject::connect(&proc, &QProcess::started, &info, &WindowInfo::start);
	return app.exec();
}
