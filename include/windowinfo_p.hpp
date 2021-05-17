#ifndef WINDOW_INFO_PRIVATE_HPP_INCLUDED
#define WINDOW_INFO_PRIVATE_HPP_INCLUDED

#include <QObject>
#include <QStringList>
#include <QTimer>

#include "xdo_wrapper.hpp"

class WindowInfoPrivate : public QObject {
	Q_OBJECT
public:
	WindowInfoPrivate(QObject *parent = nullptr);
	~WindowInfoPrivate();
public Q_SLOTS:
	void setProgram(const QString&);
	void setDebug(bool);
	void start();
	void quit();
private Q_SLOTS:
	void loop();
Q_SIGNALS:
	void hintHide();
	void focused(int x, int y, unsigned width, unsigned height);
	void unFocused();
private:
	QStringList m_ProgramSigns;	
	bool bDebug = false;
	QTimer *m_Timer = nullptr;
	XDOWrapper::xdo_t *ctx = NULL;
};

#endif // WINDOW_INFO_PRIVATE_INCLUDED_HPP
