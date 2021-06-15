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
   if(!b_RegisteredTypes) {
   	qRegisterMetaType<WId>("WId");
	qRegisterMetaType<NET::Properties>("NET::Properties");
	qRegisterMetaType<NET::Properties2>("NET::Properties2");
	b_RegisteredTypes = true;
   }
   connect(KWindowSystem::self(),
	   qOverload<WId, NET::Properties, NET::Properties2>(&KWindowSystem::windowChanged),
	   this,
	   &ActiveWindowTrackerPrivate::handleWindowChanged);

   connect(KWindowSystem::self(),
	   &KWindowSystem::windowRemoved,
	   this,
	   &ActiveWindowTrackerPrivate::handleWindowRemoved);

   connect(KWindowSystem::self(),
	   &KWindowSystem::windowAdded,
	   this,
	   &ActiveWindowTrackerPrivate::handleWindowAdded);

   // Initially no signal will be emitted so.
   handleWindowAdded(KWindowSystem::activeWindow());
#endif // LINUX
}

void ActiveWindowTrackerPrivate::stop() {
#ifdef Q_OS_LINUX
	this->disconnect();
#endif // LINUX

}

#ifdef Q_OS_LINUX
void ActiveWindowTrackerPrivate::updateActiveWindowX(WId id) {
	if(id == KWindowSystem::activeWindow()) {
	   auto properties = NET::WMGeometry | NET::WMState | NET::WMFrameExtents;

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

	   emit update(geo);
	   return;
	}
	
	updateActiveWindowX(KWindowSystem::activeWindow());
}

void ActiveWindowTrackerPrivate::handleWindowChanged(WId id, 
						     NET::Properties prop1, 
						     NET::Properties2 prop2) {
	Q_UNUSED(prop2);
	Q_UNUSED(prop1);
	updateActiveWindowX(id);
}

void ActiveWindowTrackerPrivate::handleWindowRemoved(WId id) {
	updateActiveWindowX(id);	
}

void ActiveWindowTrackerPrivate::handleWindowAdded(WId id) {
	updateActiveWindowX(id);
}
#endif // LINUX
