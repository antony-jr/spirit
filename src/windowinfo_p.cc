/// C libs
#include <cstdlib>

/// local headers
#include "windowinfo_p.hpp"

static bool windowSupported(const QString &s) {
	return s.contains("konsole", Qt::CaseInsensitive) ||
	       s.contains("terminal", Qt::CaseInsensitive);
}

WindowInfoPrivate::WindowInfoPrivate(QObject *parent) 
	: QObject(parent) {
	if(!(xdo_ctx = XDOWrapper::xdo_new(NULL))) {
		return;
	}

	m_Timer.setSingleShot(false);
	m_Timer.setInterval(200);

	connect(&m_Timer, &QTimer::timeout,
		 this, &WindowInfoPrivate::loop);

	m_Timer.start();
}

WindowInfoPrivate::~WindowInfoPrivate() {
	XDOWrapper::xdo_free(xdo_ctx);
}

void WindowInfoPrivate::quit() {
	m_Timer.stop();
}

void WindowInfoPrivate::loop() {
	Window wid = 0;
	XDOWrapper::xdo_get_focused_window(xdo_ctx, &wid);

	int name_len = 0;
	int type = 0;;
	unsigned char *name = NULL;

	XDOWrapper::xdo_get_window_name(xdo_ctx, wid,
				    &name, &name_len,
				    &type);

	QString title;
	{
		QByteArray ba((const char*)name, name_len);
		title = QString::fromUtf8(ba);
	}
	free(name);

	m_WID = wid;

	//// If the window name is in list of supported terminals for spirit 
	//// then emit the update if no, emit hintHide()
	if(windowSupported(title)) {
		int x = nX, y = nY;
		unsigned w = nWidth, h = nHeight;

		XDOWrapper::xdo_get_window_location(xdo_ctx, m_WID, &x, &y, NULL);
		XDOWrapper::xdo_get_window_size(xdo_ctx, m_WID, &w, &h);

		bool updated = false;
		if(x != nX) {
			nX = x;
			updated = true;
		}
		if(y != nY) {
			nY = y;
			updated = true;
		}
		if(w != nWidth) {
			nWidth = w;
			updated = true;
		}
		if(h != nHeight) {
			nHeight = h;
			updated = true;
		}

		if(updated) {
			emit update(
				nX,
				nY,
				nWidth,
				nHeight);
		}
	} else {
		emit hintHide();
		return;
	}
}
