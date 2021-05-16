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
	void setGuessYOffset(bool);
	void setProgram(const QString&);
	void setDebug(bool);
	void start();
	void quit();
Q_SIGNALS:
	void setYOffset(int);
	void hintHide();
	void focused(int x, int y, unsigned width, unsigned height);
	void unFocused();
private:
	QThread *m_WorkerThread;
	WindowInfoPrivate *m_Private;
};

#endif // WINDOW_INFO_HPP_INCLUDED

