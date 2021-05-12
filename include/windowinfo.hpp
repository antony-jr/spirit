#ifndef WINDOW_INFO_HPP_INCLUDED
#define WINDOW_INFO_HPP_INCLUDED
#include <QObject>
#include <QThread>

class WindowInfoPrivate;
class WindowInfo : public QObject {
	Q_OBJECT
public:
	WindowInfo(QObject *parent = nullptr);
	~WindowInfo();
public Q_SLOTS:
	void quit();
Q_SIGNALS:
	void hintHide();
	void update(int x, int y, unsigned w, unsigned h);
private:
	QThread *m_WorkerThread;
	WindowInfoPrivate *m_Private;
};

#endif // WINDOW_INFO_HPP_INCLUDED

