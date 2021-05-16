#include <QApplication>
#include <QFileInfo>
#include <QCommandLineParser>

#include "termcolor.hpp"
#include "windowinfo.hpp"
#include "spirit.hpp"

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
		  << "Usage: " << prog << " [OPTIONS]"
		  << termcolor::reset
		  << "\n\n";

	std::cout << termcolor::bold
		  << "Options:\n\n"
		  << termcolor::reset
		  << " -H,--help" << "\t" << "show help.\n"
		  << " -p,--program" << "\t" << "match with this program name.\n"
		  << " -x,--x-offset" << "\t" << "use this value as x offset.\n"
		  << " -y,--y-offset" << "\t" << "use this value as y offset.\n"
		  << " -w,--witdh" << "\t" << "use this as the witdh for the overlay.\n"
		  << " -h,--height" << "\t" << "use this as the height for the overlay.\n"
		  << " -g,--graphic-file" << "\t" << "use this graphic file to overlay.\n"
		  << " -e,--error-graphic" << "\t" << "use this graphic file for error graphic.\n"
		  << " -d,--debug " << "\t" << "enable debug messages.\n\n";

	std::cout << termcolor::blue
		  << termcolor::bold
		  << "Example:\n"
		  << "\t" << prog << " konsole\n"
		  << termcolor::reset;
}

static void setIntOp(
	std::function<void(int)> set,
	const QCommandLineParser &parser,
	const QCommandLineOption &op) {
	
	if(parser.isSet(op)) {
		bool ok = false;
		QString value = parser.value(op);
		int v = value.toInt(&ok);
		if(ok) {
			set(v);
		}
	}	
}

int main(int ac, char **av) {
	QApplication app(ac, av);
	QApplication::setQuitOnLastWindowClosed(false);
	
	QCommandLineParser parser;

	QCommandLineOption helpOption(
			QStringList() << "H"
				      << "help", 
			QCoreApplication::translate("main", "Show help."));
	parser.addOption(helpOption);

	QCommandLineOption debugOption(
			QStringList() << "d"
				      << "debug", 
			QCoreApplication::translate("main", "Show progress during copy"));
	parser.addOption(debugOption);

	QCommandLineOption programOption(QStringList() << "p" << "program",
            QCoreApplication::translate("main", "Match with this program name."),
	    "program");
	parser.addOption(programOption);
	
	QCommandLineOption xOffOption(QStringList() << "x" << "x-offset",
            QCoreApplication::translate("main", "Offset to use in x coordinate."),
	    "xoffset");
	parser.addOption(xOffOption);
	
	QCommandLineOption yOffOption(QStringList() << "y" << "y-offset",
            QCoreApplication::translate("main", "Offset to use in y coordinate."),
	    "yoffset");
	parser.addOption(yOffOption);

	QCommandLineOption widthOption(QStringList() << "w" << "width",
            QCoreApplication::translate("main", "Use this value as width."),
	    "width");
	parser.addOption(widthOption);

	QCommandLineOption heightOption(QStringList() << "h" << "height",
            QCoreApplication::translate("main", "Use this value as height."),
	    "height");
	parser.addOption(heightOption);

	QCommandLineOption gifPathOption(QStringList() << "g" << "graphic-file",
            QCoreApplication::translate("main", "Use this graphic file to overlay."),
	    "graphicfile");
	parser.addOption(gifPathOption);
	
	QCommandLineOption errPathOption(QStringList() << "e" << "error-graphic",
            QCoreApplication::translate("main", "Use this graphic file to overlay when a command errors."),
	    "errorfile");
	parser.addOption(errPathOption);

	parser.process(app);

	if(parser.isSet(helpOption)) {
		usage(av[0]);
		return 0;
	}

	bool debug = parser.isSet(debugOption);
	QString filename = QString::fromUtf8(":default.gif"),
		error_file = QString::fromUtf8(":default.gif");

	if(parser.isSet(gifPathOption)) {
		filename = parser.value(gifPathOption);
	}

	if(parser.isSet(errPathOption)) {
		error_file = parser.value(errPathOption);
	}

	QFileInfo finfo(filename);

	if(!finfo.isFile() || !finfo.exists()) {
		std::cout << termcolor::red << termcolor::bold << "Fatal: cannot open file " << av[1] 
			  << termcolor::reset << "\n";	
		return -1;
	}

	Spirit s;
	s.setGraphic(filename, false);

	/// Set Offsets and Dimensions
	setIntOp([&s](int value) -> void {
		s.setXOffset(value);
	}, parser, xOffOption);
	
	setIntOp([&s](int value) -> void {
		s.setYOffset(value);
	}, parser, yOffOption);

	setIntOp([&s](int value) -> void {
		s.setWidth(value);
	}, parser, widthOption);

	setIntOp([&s](int value) -> void {
		s.setHeight(value);
	}, parser, heightOption);
	/// ---
	
	s.setDebug(debug);

	WindowInfo info;
	if(parser.isSet(programOption)) {
		info.setProgram(
			parser.value(programOption));
	}
	info.setDebug(debug);
	
	QObject::connect(&info, &WindowInfo::focused, &s, &Spirit::update);
	//QObject::connect(&info, &WindowInfo::unFocused, &s, &Spirit::onTop);
	QObject::connect(&info, &WindowInfo::hintHide, &s, &Spirit::hide);
	
	info.start();
	return app.exec();
}
