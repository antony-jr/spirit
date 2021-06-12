#include <QRect>
#include <QDebug>

#include "activewindowtracker_p.hpp"

ActiveWindowTrackerPrivate::ActiveWindowTrackerPrivate(QObject *parent)
	: QObject(parent) {
}

ActiveWindowTrackerPrivate::~ActiveWindowTrackerPrivate() {
	stop();
}

void ActiveWindowTrackerPrivate::start() { 
#ifdef Q_OS_LINUX
   connect(KWindowSystem::self(),
	   qOverload<WId, NET::Properties, NET::Properties2>(&KWindowSystem::windowChanged),
	   this,
	   &ActiveWindowTrackerPrivate::handleWindowChanged,
	   Qt::DirectConnection);
#endif // LINUX

}

void ActiveWindowTrackerPrivate::stop() {
#ifdef Q_OS_LINUX
	this->disconnect();
#endif // LINUX

}

#ifdef Q_OS_LINUX
void ActiveWindowTrackerPrivate::handleWindowChanged(WId id, 
						     NET::Properties prop1, 
						     NET::Properties2 prop2) {
	Q_UNUSED(prop2);
	if(id == KWindowSystem::activeWindow()) {
	   auto properties = prop1 | NET::WMGeometry | NET::WMState | NET::WMFrameExtents;

	   KWindowInfo info(id, properties);
	   if(!info.valid()) {
	   	emit hide();	   
		return;
	   }
	
	   auto state = info.state();
	   if(state != NET::State::Focused) {
		emit hide();
	 	return;
	   }
	
	   auto geo = info.frameGeometry();
	   auto point = geo.topLeft();

	   emit update(point.x(), point.y());


	}	
}
#endif // LINUX
