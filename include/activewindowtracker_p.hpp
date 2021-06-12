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
	void handleWindowChanged(WId, NET::Properties, NET::Properties2);
#endif // LINUX

Q_SIGNALS:
	void update(int, int);
	void hide();
};

#endif //ACTIVE_WINDOW_TRACKER_HPP_PRIVATE
