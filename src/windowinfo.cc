#include "windowinfo_p.hpp"
#include "windowinfo.hpp"
#include "helpers_p.hpp"

WindowInfo::WindowInfo(QObject *parent)
	: QObject(parent) {
		
	m_WorkerThread = new QThread;
	m_WorkerThread->start();

	m_Private = new WindowInfoPrivate;
	m_Private->moveToThread(m_WorkerThread);
	
	connect(m_Private, &WindowInfoPrivate::yOffHint,
		this, &WindowInfo::yOffHint, Qt::DirectConnection);

	connect(m_Private, &WindowInfoPrivate::hintHide,
		this, &WindowInfo::hintHide, Qt::DirectConnection);

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


void WindowInfo::setProgram(const QString &program) {
  getMethod(m_Private, "setProgram(QString)")
    .invoke(m_Private, Qt::QueuedConnection, Q_ARG(QString, program));
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
