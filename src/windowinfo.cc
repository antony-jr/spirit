#include "windowinfo_p.hpp"
#include "windowinfo.hpp"
#include "helpers_p.hpp"

WindowInfo::WindowInfo(QObject *parent)
	: QObject(parent) {
		
	m_WorkerThread = new QThread;
	m_WorkerThread->start();

	m_Private = new WindowInfoPrivate;
	m_Private->moveToThread(m_WorkerThread);

	connect(m_Private, &WindowInfoPrivate::hintHide,
		 this, &WindowInfo::hintHide, 
		 Qt::DirectConnection);

	connect(m_Private, &WindowInfoPrivate::update,
		 this, &WindowInfo::update, 
		 Qt::DirectConnection);

}

WindowInfo::~WindowInfo() {
	quit();

	m_WorkerThread->quit();
	m_WorkerThread->wait();

	/// Delete resources
	m_WorkerThread->deleteLater();
	m_WorkerThread->deleteLater();
}

void WindowInfo::quit() {
  getMethod(m_Private, "quit()")
    .invoke(m_Private, Qt::QueuedConnection);
}

