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
#include "spiritworker.hpp"
#include "spirit.hpp"

int main(int ac, char **av) {
	QApplication app(ac, av);
	QApplication::setQuitOnLastWindowClosed(false);

	Spirit spirit;
	SpiritWorker worker;
	worker.setSpiritFile(":default.spirit");
	ActiveWindowTracker tracker;

	QObject::connect(&worker, &SpiritWorker::action, &spirit, &Spirit::animate, Qt::QueuedConnection);
	QObject::connect(&spirit, &Spirit::requestAction, &worker, &SpiritWorker::setAction, Qt::QueuedConnection);

	QObject::connect(&tracker, &ActiveWindowTracker::update, &spirit, &Spirit::update, Qt::QueuedConnection);

	QObject::connect(&tracker, &ActiveWindowTracker::hide, &spirit, &Spirit::hide, Qt::QueuedConnection);

	QObject::connect(&worker, &SpiritWorker::initialized, [&tracker, &spirit](QJsonObject info) {
	      qDebug() << "Started.";
	      auto positions = info["positions"].toObject();
	      spirit.setYOffset(positions["yoff-px"].toInt());
              //spirit.setPosition(Spirit::Position::TopRight);
	      tracker.start();	      
	});
	

	QObject::connect(&tracker, &ActiveWindowTracker::error,
	[&app](short code) { 
	   (void)code;
	   qDebug() << "Active Window Tracker Error.";
	   app.quit();
	});

	QObject::connect(&worker, &SpiritWorker::error,
	[&app](short code) {
	   qDebug() << "CODE:: " << code;
	   qDebug() << "Spirit Error.";
	   app.quit();
	});

	QObject::connect(&worker, &SpiritWorker::status,
	[&app](short code) { 
	   qDebug() << "STATUS:: " << code;
	});

	worker.init();	
	return app.exec();
}
