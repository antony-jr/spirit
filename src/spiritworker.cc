#include "spiritworker.hpp"
#include "helpers_p.hpp"

SpiritWorker::SpiritWorker(QObject *parent)
   : QObject(parent) {
   m_WorkerThread.reset(new QThread);
   m_WorkerThread->start();

   m_Worker.reset(new SpiritWorkerPrivate);
   m_Worker->moveToThread(m_WorkerThread.data());

   connect(m_Worker.data(), &SpiritWorkerPrivate::status,
	   this, &SpiritWorker::status, Qt::DirectConnection);
   
   connect(m_Worker.data(), &SpiritWorkerPrivate::started,
	   this, &SpiritWorker::started, Qt::DirectConnection);
   
   connect(m_Worker.data(), &SpiritWorkerPrivate::canceled,
	   this, &SpiritWorker::canceled, Qt::DirectConnection);
   
   connect(m_Worker.data(), &SpiritWorkerPrivate::error,
	   this, &SpiritWorker::error, Qt::DirectConnection);
  
   connect(m_Worker.data(), &SpiritWorkerPrivate::frame,
	   this, &SpiritWorker::frame, Qt::DirectConnection);
   
   connect(m_Worker.data(), &SpiritWorkerPrivate::actions,
	   this, &SpiritWorker::actions, Qt::DirectConnection);

   connect(m_Worker.data(), &SpiritWorkerPrivate::action,
	   this, &SpiritWorker::action, Qt::DirectConnection);

   connect(m_Worker.data(), &SpiritWorkerPrivate::actionChanged,
	   this, &SpiritWorker::actionChanged, Qt::DirectConnection);

   connect(m_Worker.data(), &SpiritWorkerPrivate::info,
	   this, &SpiritWorker::info, Qt::DirectConnection);
}

SpiritWorker::~SpiritWorker() {
   m_Worker->disconnect();
   m_Worker->cancel();

   m_WorkerThread->quit();
   m_WorkerThread->wait();
}

void SpiritWorker::setSpiritFile(const QString &file) {
   getMethod(m_Worker.data(), "setSpiritFile(const QString&)")
      .invoke(m_Worker.data(),
	    	Qt::QueuedConnection,
		Q_ARG(QString, AppImagePath));
}


void SpiritWorker::setAction(const QString &action) {
   getMethod(m_Worker.data(), "setAction(const QStirng&)")
      .invoke(m_Worker.data(),
	    	Qt::QueuedConnection,
		Q_ARG(QString, action));
}

void SpiritWorker::getActions() {
   getMethod(m_Worker.data(), "getActions()")
      .invoke(m_Worker.data(), Qt::QueuedConnection);
}

void SpiritWorker::getCurrentAction() {
   getMethod(m_Worker.data(), "getCurrentAction()")
      .invoke(m_Worker.data(), Qt::QueuedConnection);

}

void SpiritWorker::getInfo() {
   getMethod(m_Worker.data(), "getInfo()")
      .invoke(m_Worker.data(), Qt::QueuedConnection);
}

void SpiritWorker::start() {
   getMethod(m_Worker.data(), "start()")
      .invoke(m_Worker.data(), Qt::QueuedConnection);
}

void SpiritWorker::cancel() {
   getMethod(m_Worker.data(), "cancel()")
      .invoke(m_Worker.data(), Qt::QueuedConnection);
}
