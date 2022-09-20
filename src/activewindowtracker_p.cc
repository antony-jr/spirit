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
    if(KWindowSystem::platform() != KWindowSystem::Platform::X11) {
        /// We need to handle this specially since there is no common api
        /// to get active window in wayland, we might need to DE specific stuff
        /// which is very painful.
        /// For now we will only support GNOME (Wayland) since it supports getting
        /// active window via gdbus command.

        emit error(Error::UnSupportedPlatform);
        return;
    }
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

void ActiveWindowTrackerPrivate::rescan() {
#ifdef Q_OS_LINUX
    updateActiveWindowX(KWindowSystem::activeWindow());
#endif // LINUX 
}

void ActiveWindowTrackerPrivate::stop() {
#ifdef Q_OS_LINUX
    this->disconnect();
#endif // LINUX

}

void ActiveWindowTrackerPrivate::addAllowedProgram(QString program) {
    m_AllowedPrograms.append(program);
}

void ActiveWindowTrackerPrivate::removeAllowedProgram(int pos) {
    if (pos < m_AllowedPrograms.size() &&
            pos >= 0) {
        m_AllowedPrograms.removeAt(pos);
        emit allowedPrograms(m_AllowedPrograms);
    }
}

void ActiveWindowTrackerPrivate::getAllowedPrograms() {
    emit allowedPrograms(m_AllowedPrograms);
}

#ifdef Q_OS_LINUX
void ActiveWindowTrackerPrivate::updateActiveWindowX(WId id) {
    if(id == KWindowSystem::activeWindow()) {
        auto properties = NET::WMGeometry | NET::WMState | NET::WMFrameExtents | NET::WMVisibleName;

        KWindowInfo info(id, properties);
        if(!info.valid()) {
            emit hide();
            return;
        }

        auto allowed = m_AllowedPrograms.isEmpty();
        auto title = info.visibleName();
        for (auto prog : m_AllowedPrograms) {
            if (title.contains(prog)) {
                allowed = true;
                break;
            }
        }

        if (!allowed) {
            emit hide();
            return;
        }

        auto state = info.state();
        if(!(state & NET::State::Focused)) {
            emit hide();
            return;
        }

        if(state & NET::State::SkipTaskbar ||
                state & NET::State::Modal ||
                state & NET::State::KeepBelow) {
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
