#ifndef ACTIVE_WINDOW_TRACKER_HPP
#define ACTIVE_WINDOW_TRACKER_HPP
#include <QObject>
#include <QRect>

#include "spiritenums.hpp"

class QThread;
class ActiveWindowTrackerPrivate;
class ActiveWindowTracker : public QObject {
	Q_OBJECT
public:
	struct Error : public SpiritEnums::ActiveWindowTracker::Error { };

	ActiveWindowTracker(QObject *parent = nullptr);
	~ActiveWindowTracker();
Q_SIGNALS:
	void error(short);
	void update(QRect);
	void hide();
public Q_SLOTS:
	void start();
	void stop();
private:
	ActiveWindowTrackerPrivate *d;	
	QThread *worker;
};

#endif // ACTIVE_WINDOW_TRACKER_HPP
