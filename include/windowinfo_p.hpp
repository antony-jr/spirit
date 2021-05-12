#ifndef WINDOW_INFO_PRIVATE_HPP_INCLUDED
#define WINDOW_INFO_PRIVATE_HPP_INCLUDED

#include <QObject>
#include <QTimer>

#include "xdo_wrapper.hpp"

class WindowInfoPrivate : public QObject {
	Q_OBJECT
public:
	WindowInfoPrivate(QObject *parent = nullptr);
	~WindowInfoPrivate();
public Q_SLOTS:
	void quit();

private Q_SLOTS:
	void loop();

Q_SIGNALS:
	//// Request the main overlay to hide.
	void hintHide();

	//// If emitted this implies to show the overlay if
	//// it is hidden.
	void update(
		int x,
		int y,
		unsigned width,
		unsigned height);
private:
	QTimer m_Timer;
	Window m_WID = 0;
	int nX = 0, nY = 0;
	unsigned nWidth = 0, nHeight = 0;	
	XDOWrapper::xdo_t *xdo_ctx = NULL;
};

#endif // WINDOW_INFO_PRIVATE_INCLUDED_HPP
