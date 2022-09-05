#include "spiritdaemon.hpp"
#include "spiritdaemon_p.hpp"
#include "helpers_p.hpp"

SpiritDaemon::SpiritDaemon(QObject *parent)
   : QObject(parent) {
   m_WorkerThread = new QThread;
   m_WorkerThread->start();

   m_Worker = new SpiritDaemonPrivate;
   m_Worker->moveToThread(m_WorkerThread);

   connect(m_Worker, &SpiritDaemonPrivate::quit,
	   this, &SpiritDaemon::quit, Qt::DirectConnection);

connect(m_Worker, &SpiritDaemonPrivate::pause,
	   this, &SpiritDaemon::pause, Qt::DirectConnection);
connect(m_Worker, &SpiritDaemonPrivate::resume,
	   this, &SpiritDaemon::resume, Qt::DirectConnection);

connect(m_Worker, &SpiritDaemonPrivate::setAction,
	   this, &SpiritDaemon::setAction, Qt::DirectConnection);

connect(m_Worker, &SpiritDaemonPrivate::resetAction,
	   this, &SpiritDaemon::resetAction, Qt::DirectConnection);

connect(m_Worker, &SpiritDaemonPrivate::setSpirit,
	   this, &SpiritDaemon::setSpirit, Qt::DirectConnection);

connect(m_Worker, &SpiritDaemonPrivate::unsetSpirit,
	   this, &SpiritDaemon::unsetSpirit, Qt::DirectConnection);

connect(m_Worker, &SpiritDaemonPrivate::setPosition,
	   this, &SpiritDaemon::setPosition, Qt::DirectConnection);

connect(m_Worker, &SpiritDaemonPrivate::setScale,
	   this, &SpiritDaemon::setScale, Qt::DirectConnection);

connect(m_Worker, &SpiritDaemonPrivate::setSpeed,
	   this, &SpiritDaemon::setSpeed, Qt::DirectConnection);

}

SpiritDaemon::~SpiritDaemon() {
   m_Worker->disconnect();
   stop();

   m_WorkerThread->quit();
   m_WorkerThread->wait();

   m_Worker->deleteLater();
   m_WorkerThread->deleteLater();
}

void SpiritDaemon::run() {
   getMethod(m_Worker, "run()")
      .invoke(m_Worker,
	    	Qt::QueuedConnection);
}

void SpiritDaemon::stop() {
   getMethod(m_Worker, "stop()")
      .invoke(m_Worker,
	    	Qt::QueuedConnection);
}

void SpiritDaemon::updateAction(QString action) {
   getMethod(m_Worker, "updateAction(QString)")
      .invoke(m_Worker,
	    	Qt::QueuedConnection, Q_ARG(QString, action));
}

void SpiritDaemon::updateSpirit(QString spirit) {
   getMethod(m_Worker, "updateSpirit(QString)")
      .invoke(m_Worker,
	    	Qt::QueuedConnection, Q_ARG(QString, spirit));
}

void SpiritDaemon::updateSpiritMeta(QJsonObject meta) {
   getMethod(m_Worker, "updateSpiritMeta(QJsonObject)")
      .invoke(m_Worker,
	    	Qt::QueuedConnection, Q_ARG(QJsonObject, meta));
}
