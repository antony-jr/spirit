#include "spiritworker.hpp"
#include "spiritworker_p.hpp"
#include "helpers_p.hpp"

SpiritWorker::SpiritWorker(QObject *parent)
   : QObject(parent) {
   m_WorkerThread = new QThread;
   m_WorkerThread->start();

   m_Worker = new SpiritWorkerPrivate;
   m_Worker->moveToThread(m_WorkerThread);

   connect(m_Worker, &SpiritWorkerPrivate::status,
	   this, &SpiritWorker::status, Qt::DirectConnection);
  
   connect(m_Worker, &SpiritWorkerPrivate::initialized,
	   this, &SpiritWorker::initialized, Qt::DirectConnection);

   connect(m_Worker, &SpiritWorkerPrivate::started,
	   this, &SpiritWorker::started, Qt::DirectConnection);
   
   connect(m_Worker, &SpiritWorkerPrivate::canceled,
	   this, &SpiritWorker::canceled, Qt::DirectConnection);
   
   connect(m_Worker, &SpiritWorkerPrivate::error,
	   this, &SpiritWorker::error, Qt::DirectConnection);
  
   connect(m_Worker, &SpiritWorkerPrivate::actions,
	   this, &SpiritWorker::actions, Qt::DirectConnection);

   connect(m_Worker, &SpiritWorkerPrivate::action,
	   this, &SpiritWorker::action, Qt::DirectConnection);

   connect(m_Worker, &SpiritWorkerPrivate::info,
	   this, &SpiritWorker::info, Qt::DirectConnection);
}

SpiritWorker::~SpiritWorker() {
   m_Worker->disconnect();
   cancel();

   m_WorkerThread->quit();
   m_WorkerThread->wait();

   m_Worker->deleteLater();
   m_WorkerThread->deleteLater();
}

void SpiritWorker::setSpiritFile(const QString &file) {
   getMethod(m_Worker, "setSpiritFile(const QString&)")
      .invoke(m_Worker,
	    	Qt::QueuedConnection,
		Q_ARG(QString, file));
}


void SpiritWorker::setAction(const QString &action) {
   qDebug() << "setAction: " << action;
   getMethod(m_Worker, "setAction(const QString&)")
      .invoke(m_Worker,
	    	Qt::QueuedConnection,
		Q_ARG(QString, action));
}

void SpiritWorker::getActions() {
   getMethod(m_Worker, "getActions()")
      .invoke(m_Worker, Qt::QueuedConnection);
}

void SpiritWorker::getCurrentAction() {
   getMethod(m_Worker, "getCurrentAction()")
      .invoke(m_Worker, Qt::QueuedConnection);

}

void SpiritWorker::getInfo() {
   getMethod(m_Worker, "getInfo()")
      .invoke(m_Worker, Qt::QueuedConnection);
}

void SpiritWorker::init() {
   getMethod(m_Worker, "init()")
      .invoke(m_Worker, Qt::QueuedConnection);
}

void SpiritWorker::cancel() {
   getMethod(m_Worker, "cancel()")
      .invoke(m_Worker, Qt::QueuedConnection);
}
