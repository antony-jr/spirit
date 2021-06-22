#include <QApplication>
#include <QTimer>
#include <QThread>
#include <QProcess>
#include <QLocalServer>
#include <QLocalSocket>
#include <QFileInfo>
#include <QDir>
#include <QCommandLineParser>
#include <cstdlib>

#include <iostream>

#include "activewindowtracker.hpp"
#include "spirit.hpp"

int main(int ac, char **av) {
	if(ac == 1) {
		std::cout << "Usage: spirit <Spirit File>\n";
		return 0;
	}
	QApplication app(ac, av);
	QApplication::setQuitOnLastWindowClosed(false);

	QString filePath(av[1]);
	Spirit spirit(filePath);

	ActiveWindowTracker tracker;

	QObject::connect(&tracker, &ActiveWindowTracker::update, &spirit, &Spirit::update);
	QObject::connect(&tracker, &ActiveWindowTracker::hide, &spirit, &Spirit::hide);
	
	tracker.start();
	return app.exec();
}
