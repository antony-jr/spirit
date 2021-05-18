#include <QApplication>
#include <QStyleFactory>
#include <QTimer>
#include <QThread>
#include <QProcess>
#include <QLocalServer>
#include <QLocalSocket>
#include <QFileInfo>
#include <QDir>
#include <QCommandLineParser>
#include <cstdlib>

#include "termcolor.hpp"
#include "windowinfo.hpp"
#include "spirit.hpp"
#include "bashrcwriter.hpp"

static void info() {
	std::cout << termcolor::bold << "Spirit"
		  << termcolor::reset
		  << " " 
		  << termcolor::underline
		  << "v0.1.2"
		  << termcolor::reset
		  << ","
		  << " Overlay gif over any XWindow."
		  << "\n"
		  << termcolor::magenta
		  << termcolor::bold
		  << "D. Antony J.R <antonyjr@pm.me>"
		  << termcolor::reset
		  << "\n\n";
}

static void usage(const char *prog) {
	info();

	std::cout << termcolor::bold
		  << "Usage: " << prog << " <SUBCOMMAND> [OPTIONS]"
		  << termcolor::reset
		  << "\n\n";

	std::cout << termcolor::bold
		  << "Subcommands:\n\n"
		  << " init" << "\t" << "Initialize Spirit.\n"
		  << " deinit" << "\t" << "Kill Spirit.\n"
		  << " error" << "\t" << "Set error state.\n"
		  << " nonerror" << "\t" << "Set normal state.\n\n";

	std::cout << termcolor::bold
		  << "Init Subcommand Options:\n\n"
		  << termcolor::reset
		  << " -H,--help" << "\t" << "show help.\n"
		  << " -a,--h-align" << "\t" << "Possible values center,lef, and right.\n"
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
		  << "\t" << prog << " init\n"
		  << "\t" << prog << " kill\n"
		  << "\t" << prog << " init -p dolphin"
		  << termcolor::reset
		  << "\n";
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
	if(ac == 1) {
		usage(av[0]);
		return 0;
	}
	QApplication app(ac, av);
	QApplication::setQuitOnLastWindowClosed(false);
	QApplication::setStyle(QStyleFactory::create("Fusion"));

	QCommandLineParser parser;

	parser.addPositionalArgument("subcommand", 
		QCoreApplication::translate("main", "subcommand"));

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

	QCommandLineOption hAlignOption(QStringList() << "a" << "h-align",
            QCoreApplication::translate("main", "Aligns the spirit horizontally center,left and right."),
	    "halign");
	parser.addOption(hAlignOption);

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

	const QStringList args = parser.positionalArguments();
	if(args.size() == 0) {
		usage(av[0]);
		return 0;
	}

	QString programPath;
	auto appimage = std::getenv("APPIMAGE");
	if(appimage) {
		programPath = QString::fromUtf8(appimage);
	}else {
		programPath = QCoreApplication::applicationFilePath();
	}

	BashRCWriter bashrc_writer;
	bashrc_writer.setProgram(programPath);

	const QString subcommand = args.at(0).toLower();
	const QString socketName = QString::fromUtf8("com.github.antony-jr.spirit");
	QLocalSocket socket;

	//// Process deinit,check and reinit.
	if(subcommand == "deinit") {
		info();
		/// Send kill via Unix Sockets.
		socket.connectToServer(socketName);
		socket.waitForConnected();
		if(socket.state() == QLocalSocket::ConnectedState) {
			/// Quit the previous one.
			socket.write("quit");
			socket.waitForDisconnected();
		}else {
			std::cout << termcolor::bold
				  << termcolor::red
				  << "ERROR: no running spirit process found."
				  << termcolor::reset
				  << "\n";
			return -1;
		}
	
		std::cout << termcolor::bold
			  << "Please restart your Terminal."
			  << termcolor::reset
			  << "\n";

		std::cout << termcolor::bold
			  << termcolor::cyan
			  << "Spirit De-Initialized Successfully."
			  << termcolor::reset
			  << "\n";
		return 0;
	}else if(subcommand == "error" || subcommand == "nonerror") {
		/// Send previous command return code via Unix Sockets.
		socket.connectToServer(socketName);
		socket.waitForConnected();
		if(socket.state() == QLocalSocket::ConnectedState) {
			/// Quit the previous one.
			if(subcommand == "error") {
				socket.write("check 1");
			}else {
				socket.write("check 0");
			}
			socket.waitForDisconnected();
		}else {
			return -1;
		}
		return 0;	
	}else if(subcommand == "init") {
		/// Check if program is running if so quit it.
		socket.connectToServer(socketName);
		socket.waitForConnected();
		if(socket.state() == QLocalSocket::ConnectedState) {
			/// Quit the previous one.
			socket.write("quit");
			socket.waitForDisconnected();
		}
	
		info();	
		auto arguments = QCoreApplication::arguments();
            
		if(arguments.isEmpty()) {
			std::cout << termcolor::bold
				  << termcolor::red
				  << "ERROR: cannot get current program binary location."
				  << termcolor::reset
				  << "\n";
		
			return -1;
		}

		arguments.removeFirst();
		arguments.replaceInStrings(
			QString::fromUtf8("init"),
			QString::fromUtf8("server"),
			Qt::CaseInsensitive);

		bool r = QProcess::startDetached(programPath, arguments);
		if(!r) {
			std::cout << termcolor::bold
				  << termcolor::red
				  << "ERROR: cannot initialize server."
				  << termcolor::reset
				  << "\n";

			return -1;
		}

		if(bashrc_writer.write()) {
			std::cout << termcolor::bold
			     << "~/.bashrc updated. Please restart your terminal for full effect."
			     << termcolor::reset
			     << "\n";
		}else {
			std::cout << "Warning: cannot update .bashrc."
				  << "\n";
		}


		std::cout << termcolor::bold
			  << termcolor::yellow
			  << "Spirit Initialized Successfully."
			  << termcolor::reset
			  << "\n";
		return 0;
	}else if(subcommand == "server") { /// Un-Documented Secret internal server init
		/// Check if program is running if so quit it.
		socket.connectToServer(socketName);
		socket.waitForConnected();
		if(socket.state() == QLocalSocket::ConnectedState) {
			socket.write("ping");
			socket.waitForDisconnected();
			return 0;
		}

	} else {
		usage(av[0]);
		return 0;
	}
	//// ---

	QLocalServer server;
	server.removeServer(socketName);
	if(!server.listen(socketName)) {
		return -1;
	}

	bool debug = parser.isSet(debugOption);
	QString filename = QString::fromUtf8(":default.gif"),
		error_file = QString::fromUtf8(":error.gif");

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

	Spirit s,
	       error;

	/// Set Offsets and Dimensions
	setIntOp([&s,&error](int value) -> void {
		s.setXOffset(value);
		error.setXOffset(value);
	}, parser, xOffOption);
	
	setIntOp([&s,&error](int value) -> void {
		s.setYOffset(value);
		error.setYOffset(value);
	}, parser, yOffOption);

	setIntOp([&s,&error](int value) -> void {
		s.setWidth(value);
		error.setWidth(value);
	}, parser, widthOption);

	setIntOp([&s,&error](int value) -> void {
		s.setHeight(value);
		error.setHeight(value);
	}, parser, heightOption);
	/// ---

	if(parser.isSet(hAlignOption)) {
		auto value = parser.value(hAlignOption).toLower();
		if(value == "center") {
			s.setHorizontalAlignment(Spirit::HAlign::Center);
			error.setHorizontalAlignment(Spirit::HAlign::Center);	
		} else if(value == "right") {
			s.setHorizontalAlignment(Spirit::HAlign::Right);	
			error.setHorizontalAlignment(Spirit::HAlign::Right);		
		}
	}	

	s.setDebug(debug);
	error.setDebug(debug);
	
	s.setGraphic(filename, false);
	error.setGraphic(error_file, false);

	WindowInfo info;
	if(parser.isSet(programOption)) {
		info.setProgram(
			parser.value(programOption));
	}
	info.setDebug(debug);

	QObject::connect(&info, &WindowInfo::yOffHint, &s, &Spirit::setYOffset);	
	QObject::connect(&info, &WindowInfo::focused, &s, &Spirit::update);
	QObject::connect(&info, &WindowInfo::hintHide, &s, &Spirit::hide);


	bool errored = false;

	QObject::connect(&server, &QLocalServer::newConnection,
	[&server, &app, &s, &error_file, &errored, &info, &filename, &error, &bashrc_writer]() {
	   auto socket = server.nextPendingConnection();
	   if(!socket || !socket->isValid()) {
	   	return;
	   }

	   socket->waitForReadyRead();

	   auto dataSent = QString::fromUtf8(socket->readAll());
	   if(!dataSent.contains("quit")) {
	   	socket->disconnectFromServer();
	   }

	   if(dataSent.contains("quit")) {
	   	bashrc_writer.unwrite();
		QObject::connect(&app, &QApplication::aboutToQuit, socket, &QLocalSocket::disconnectFromServer);
		QTimer::singleShot(500, &app, &QApplication::quit);
	   }else if(dataSent.contains("check")) {
	      auto args = dataSent.split(" ");
	      if(args.size() != 2) {
	         return;
	      }
	      
	      bool ok = false;
	      int r = args.at(1).toInt(&ok);
	      if(!ok) {
		      return;
	      }

	      if(!errored && r) {
		 errored = true;
		 QObject::disconnect(&info, &WindowInfo::yOffHint, &s, &Spirit::setYOffset);	
		 QObject::disconnect(&info, &WindowInfo::focused, &s, &Spirit::update);
		 QObject::disconnect(&info, &WindowInfo::hintHide, &s, &Spirit::hide);
		 s.hide();
		 
		 QObject::connect(&info, &WindowInfo::yOffHint, &error, &Spirit::setYOffset);	
		 QObject::connect(&info, &WindowInfo::focused, &error, &Spirit::update);
		 QObject::connect(&info, &WindowInfo::hintHide, &error, &Spirit::hide);
	      }
	      
	      if(errored && !r) {
		errored = false;

		QObject::disconnect(&info, &WindowInfo::yOffHint, &error, &Spirit::setYOffset);	
		QObject::disconnect(&info, &WindowInfo::focused, &error, &Spirit::update);
		QObject::disconnect(&info, &WindowInfo::hintHide, &error, &Spirit::hide);
		error.hide();

		QObject::connect(&info, &WindowInfo::yOffHint, &s, &Spirit::setYOffset);	
		QObject::connect(&info, &WindowInfo::focused, &s, &Spirit::update);
		QObject::connect(&info, &WindowInfo::hintHide, &s, &Spirit::hide);
	      } 
	  }
	});

	info.start();
	return app.exec();
}
