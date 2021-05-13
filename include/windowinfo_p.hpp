#ifndef WINDOW_INFO_PRIVATE_HPP_INCLUDED
#define WINDOW_INFO_PRIVATE_HPP_INCLUDED

#include <QObject>
#include <QHash>
#include <QTimer>

#include "xdo_wrapper.hpp"

class WindowInfoPrivate : public QObject {
	Q_OBJECT
public:
	WindowInfoPrivate(int, QObject *parent = nullptr);
	~WindowInfoPrivate();
public Q_SLOTS:
	void start();
	void quit();
private Q_SLOTS:
	void loop();
Q_SIGNALS:
	void hintHide();
	void windowId(long long);
	void focused(int x, int y, unsigned width, unsigned height);
	void unFocused();
private:
	int m_PID = 0;
	Window m_WID = 0;
	QTimer *m_Timer = nullptr;
	XDOWrapper::xdo_t *ctx = NULL;
};

#endif // WINDOW_INFO_PRIVATE_INCLUDED_HPP
