#ifndef ACTIVE_WINDOW_TRACKER_HPP_PRIVATE
#define ACTIVE_WINDOW_TRACKER_HPP_PRIVATE
#include <QObject>
#ifdef Q_OS_LINUX
# include <kwindowsystem.h>
# include <kwindowinfo.h>
#endif // LINUX 

class ActiveWindowTrackerPrivate : public QObject {
	Q_OBJECT
public:
	ActiveWindowTrackerPrivate(QObject *parent = nullptr);
	~ActiveWindowTrackerPrivate();
public Q_SLOTS:
	void start();
	void stop();

#ifdef Q_OS_LINUX
private Q_SLOTS:
	void updateActiveWindowX(WId);
	void handleWindowChanged(WId, NET::Properties, NET::Properties2);
	void handleWindowRemoved(WId);
	void handleWindowAdded(WId);
#endif // LINUX

Q_SIGNALS:
	void update(int, int);
	void hide();

private:
#ifdef Q_OS_LINUX
	bool b_RegisteredTypes = false;
#endif // LINUX
};

#endif //ACTIVE_WINDOW_TRACKER_HPP_PRIVATE
