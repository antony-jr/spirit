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

	int pid = XDOWrapper::xdo_get_pid_window(ctx, wid);
	if(!pid) {
		return;
	}

	if(pid == m_PID) {
		if(m_WID == 0) {
			m_WID = wid;
		}

		int x = 0,
		    y = 0;
		unsigned w = 0,
			 h = 0;
		int ret  = 0;

		emit windowId((long long)wid);

		ret = XDOWrapper::xdo_get_window_location(ctx, wid, &x, &y, NULL);
		if(ret){
			emit unFocused();
			return;
		}
		ret = XDOWrapper::xdo_get_window_size(ctx, wid, &w, &h);
		if(r) {
			emit unFocused();
			return;
		}

		emit focused(x,y,w,h);
	}else {
		{
			unsigned char *value = NULL;
			long nitems = 0;
			Atom type = 0;
			int size = 0;
			XDOWrapper::xdo_get_window_property(ctx, m_WID, "_NET_WM_STATE",
				&value, &nitems, &type, &size);

			if(size && value && type ) {
				if(value[0] != 0) {
					emit hintHide();
				}else {
					emit unFocused();
				}
				free(value);
			}
		}		
	}
}
