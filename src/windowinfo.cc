#include "windowinfo_p.hpp"
#include "windowinfo.hpp"
#include "helpers_p.hpp"

WindowInfo::WindowInfo(int pid, QObject *parent)
	: QObject(parent) {
		
	m_WorkerThread = new QThread;
	m_WorkerThread->start();

	m_Private = new WindowInfoPrivate(pid);
	m_Private->moveToThread(m_WorkerThread);
	
	connect(m_Private, &WindowInfoPrivate::hintHide,
		this, &WindowInfo::hintHide, Qt::DirectConnection);

	connect(m_Private, &WindowInfoPrivate::windowId,
		this, &WindowInfo::windowId, Qt::DirectConnection);
	
	connect(m_Private, &WindowInfoPrivate::unFocused,
		this, &WindowInfo::unFocused, Qt::DirectConnection);
	
	connect(m_Private, &WindowInfoPrivate::focused,
		this, &WindowInfo::focused, Qt::DirectConnection);
}

WindowInfo::~WindowInfo() {
	quit();
	m_Private->deleteLater();

	m_WorkerThread->quit();
	m_WorkerThread->wait();

	/// Delete resources
	m_WorkerThread->deleteLater();
}


void WindowInfo::setDebug(bool value) {
  getMethod(m_Private, "setDebug(bool)")
    .invoke(m_Private, Qt::QueuedConnection, Q_ARG(bool, value));
}

void WindowInfo::start() {
  getMethod(m_Private, "start()")
    .invoke(m_Private, Qt::QueuedConnection);
}

void WindowInfo::quit() {
  getMethod(m_Private, "quit()")
    .invoke(m_Private, Qt::QueuedConnection);
}
