#include "spiritfile.hpp"
#include "spiritfile_p.hpp"
#include "helpers_p.hpp"

SpiritFile::SpiritFile(QObject *parent)
   : QObject(parent) {
   m_WorkerThread = new QThread;
   m_WorkerThread->start();

   m_Worker = new SpiritFilePrivate;
   m_Worker->moveToThread(m_WorkerThread);

   connect(m_Worker, &SpiritFilePrivate::status,
	   this, &SpiritFile::status, Qt::DirectConnection);
   
   connect(m_Worker, &SpiritFilePrivate::initialized,
	   this, &SpiritFile::initialized, Qt::DirectConnection);
   
   connect(m_Worker, &SpiritFilePrivate::canceled,
	   this, &SpiritFile::canceled, Qt::DirectConnection);
   
   connect(m_Worker, &SpiritFilePrivate::error,
	   this, &SpiritFile::error, Qt::DirectConnection);
  
   connect(m_Worker, &SpiritFilePrivate::actions,
	   this, &SpiritFile::actions, Qt::DirectConnection);

   connect(m_Worker, &SpiritFilePrivate::action,
	   this, &SpiritFile::action, Qt::DirectConnection);

   connect(m_Worker, &SpiritFilePrivate::info,
	   this, &SpiritFile::info, Qt::DirectConnection);
}

SpiritFile::~SpiritFile() {
   m_Worker->disconnect();
   cancel();

   m_WorkerThread->quit();
   m_WorkerThread->wait();

   m_Worker->deleteLater();
   m_WorkerThread->deleteLater();
}

void SpiritFile::setSpiritFile(const QString &file) {
   getMethod(m_Worker, "setSpiritFile(const QString&)")
      .invoke(m_Worker,
	    	Qt::QueuedConnection,
		Q_ARG(QString, file));
}


void SpiritFile::setAction(const QString &action) {
   getMethod(m_Worker, "setAction(const QStirng&)")
      .invoke(m_Worker,
	    	Qt::QueuedConnection,
		Q_ARG(QString, action));
}

void SpiritFile::getActions() {
   getMethod(m_Worker, "getActions()")
      .invoke(m_Worker, Qt::QueuedConnection);
}

void SpiritFile::getCurrentAction() {
   getMethod(m_Worker, "getCurrentAction()")
      .invoke(m_Worker, Qt::QueuedConnection);

}

void SpiritFile::getInfo() {
   getMethod(m_Worker, "getInfo()")
      .invoke(m_Worker, Qt::QueuedConnection);
}

void SpiritFile::init() {
   getMethod(m_Worker, "init()")
      .invoke(m_Worker, Qt::QueuedConnection);
}

void SpiritFile::cancel() {
   getMethod(m_Worker, "cancel()")
      .invoke(m_Worker, Qt::QueuedConnection);
}
