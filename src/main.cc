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
#include "spiritdaemon.hpp"

int main(int ac, char **av) {
	QApplication app(ac, av);
	QApplication::setQuitOnLastWindowClosed(false);

	SpiritDaemon daemon;
	Spirit spirit;
	SpiritWorker worker;
	worker.setSpiritFile(":default.spirit");
	ActiveWindowTracker tracker;
	tracker.start();

	QObject::connect(&daemon, &SpiritDaemon::quit, &app, &QApplication::quit);
	QObject::connect(&tracker, &ActiveWindowTracker::hide, &spirit, &Spirit::hide, Qt::QueuedConnection);

	
	QObject::connect(&worker, &SpiritWorker::initialized, [&worker, &spirit, &tracker, &daemon](QJsonObject info) {
	      daemon.updateSpiritMeta(info); 
	      QObject::connect(&tracker, &ActiveWindowTracker::update, &spirit, &Spirit::update, Qt::QueuedConnection);
	      QObject::connect(&worker, &SpiritWorker::action, &spirit, &Spirit::animate, Qt::QueuedConnection);
	      QObject::connect(&spirit, &Spirit::requestAction, &worker, &SpiritWorker::setAction, Qt::QueuedConnection);
	      QObject::connect(&tracker, &ActiveWindowTracker::hide, &spirit, &Spirit::hide, Qt::QueuedConnection);


	     });
	

	QObject::connect(&tracker, &ActiveWindowTracker::error,
	[&app](short code) { 
	   // TODO: Report error better.
	   app.quit();
	});

	QObject::connect(&worker, &SpiritWorker::error,
	[&app](short code) {
	   // TODO: handle error better. 
	   app.quit();
	});

	worker.init();	
	

	

	daemon.run();	
	QObject::connect(&daemon, &SpiritDaemon::setAction, &worker, &SpiritWorker::setAction);
	QObject::connect(&worker, &SpiritWorker::action, &daemon, &SpiritDaemon::updateAction);

	QObject::connect(&daemon, &SpiritDaemon::setSpirit, [&tracker, &spirit, &worker, &daemon](QString loc) {
	      QObject::disconnect(&tracker, &ActiveWindowTracker::update, &spirit, &Spirit::update);
	      QObject::disconnect(&worker, &SpiritWorker::action, &spirit, &Spirit::animate);
	      QObject::disconnect(&spirit, &Spirit::requestAction, &worker, &SpiritWorker::setAction);
	      QObject::disconnect(&tracker, &ActiveWindowTracker::hide, &spirit, &Spirit::hide);



	      QEventLoop loop;


	      QObject::connect(&spirit, &Spirit::cleared, &loop, &QEventLoop::quit);
	      QTimer timer;
	      timer.setInterval(100);
	      timer.setSingleShot(true);

	      QObject::connect(&timer, &QTimer::timeout, &spirit, &Spirit::clear, Qt::QueuedConnection);

	      timer.start(); 
	      loop.exec();

	      daemon.updateSpirit(loc);

	      QObject::connect(&worker, &SpiritWorker::status, &loop, &QEventLoop::quit);

	      worker.cancel();
	      loop.exec();

	      worker.setSpiritFile(loc);
	      worker.init(); 
	      qDebug() << "Inits"; 
	});
	QObject::connect(&daemon, &SpiritDaemon::setPosition, &spirit, &Spirit::setPosition);
	QObject::connect(&daemon, &SpiritDaemon::setScale, &spirit, &Spirit::setScale);
	QObject::connect(&daemon, &SpiritDaemon::setSpeed, &spirit, &Spirit::setSpeed);
	
	return app.exec();
}
