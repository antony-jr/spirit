#include <QWindow>
#include <QFileInfo>
#include <QDebug>

/// C libs
#include <cstdlib>

/// local headers
#include "windowinfo_p.hpp"

WindowInfoPrivate::WindowInfoPrivate(int pid, QObject *parent) 
	: QObject(parent) { 
	m_PID = pid;
}

WindowInfoPrivate::~WindowInfoPrivate() {
	quit();
	XDOWrapper::xdo_free(ctx);
}

void WindowInfoPrivate::setDebug(bool value) {
	bDebug = value;
}

void WindowInfoPrivate::start() {
	ctx = XDOWrapper::xdo_new(NULL);
	if(!ctx) {
		qDebug() << "Cannot get xdo context.";
		return;
	}

	m_Timer = new QTimer(this);
	m_Timer->setSingleShot(false);
	m_Timer->setInterval(200);

	connect(m_Timer, &QTimer::timeout,
		 this, &WindowInfoPrivate::loop);

	m_Timer->start();
}

void WindowInfoPrivate::quit() {
	if(!m_Timer) {
		return;
	}
	if(!m_Timer->isActive()) {
		return;
	}

	m_Timer->stop();
}

void WindowInfoPrivate::loop() {
	Window wid = 0;
	int r = XDOWrapper::xdo_get_focused_window_sane(ctx, &wid);
	if(r) {
		return;
	}
	if(bDebug) {
		qDebug() << "WindowInfo::loop():: " 
			 << "PID(" << m_PID << "):: " 
			 << "Window in Focus:: "
			 << wid;
	}
	
	int pid = XDOWrapper::xdo_get_pid_window(ctx, wid);
	if(!pid) {
		if(bDebug) {
			qDebug() << "WindowInfo::loop():: " 
				 << "PID(" << m_PID << "):: " 
				 << "Cannot get pid of window "
				 << wid
				 << ".";
		}
		return;
	}

	if(pid == m_PID) {
		if(m_WID == 0) {
			m_WID = wid;
		}

		if(bDebug) {
			qDebug() << "WindowInfo::loop():: " 
				 << "PID(" << m_PID << "):: " 
				 << "Focused Window is target PID.";
		}
	
		int x = 0,
		    y = 0;
		unsigned w = 0,
			 h = 0;
		int ret  = 0;

		emit windowId((long long)wid);

		ret = XDOWrapper::xdo_get_window_location(ctx, wid, &x, &y, NULL);
		if(ret){
			if(bDebug) {
				qDebug() << "WindowInfo::loop():: " 
					<< "PID(" << m_PID << "):: " 
					<< "Cannot get Window location.";
			}
			emit unFocused();
			return;
		}
		ret = XDOWrapper::xdo_get_window_size(ctx, wid, &w, &h);
		if(ret) {
			if(bDebug) {
				qDebug() << "WindowInfo::loop():: " 
					<< "PID(" << m_PID << "):: " 
					<< "Cannot get Window szie.";
	
			}
			emit unFocused();
			return;
		}

		if(bDebug) {
			unsigned char *name = NULL;
			int name_len = 0;
			int name_type = 0;

			ret = XDOWrapper::xdo_get_window_name(
				ctx,
				wid,
				&name,
				&name_len,
				&name_type);

			QString title;

			if(!ret) {
				{
					QByteArray array((const char*)name, name_len);
					title = QString(array);

				}
				free(name);
			}

			qDebug() << "WindowInfo::loop():: " 
				 << "PID(" << m_PID << "):: " 
				 << "Title -> " 
				 << title
				 << ", X: "
				 << x 
				 << " Y: "
				 << y
				 << " Width: "
				 << w
				 << " Height: "
				 << h
				 << "." 
				 ;
		}

		emit focused(x,y,w,h);
	}else {
		if(bDebug) {
			qDebug() << "WindowInfo::loop():: " 
				 << "PID(" << m_PID << "):: " 
				 << "Focused Window pid is " << pid;
		}
		emit hintHide();
	}
}
